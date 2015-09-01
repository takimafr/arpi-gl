/*
 * Copyright (C) 2015  eBusiness Information
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package mobi.designmyapp.arpigl.util;

import android.util.Log;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import mobi.designmyapp.arpigl.mapper.Mapper;
import mobi.designmyapp.arpigl.model.Tile;

public final class TileCache<T> {

    private static final String TAG = TileCache.class.getSimpleName();
    private static final Pattern FILE_PATTERN = Pattern.compile("(\\d+)/(\\d+)/(\\d+)\\.png$");

    private String mTileCacheRoot;
    private CachedLinkedHashSet<Tile.Id> mManagedTiles;
    private Pattern mPattern;
    private String mExtension;

    public TileCache(String tileCacheRoot, Pattern pattern, String extension, int size) {
        mTileCacheRoot = tileCacheRoot;
        if (mTileCacheRoot.charAt(mTileCacheRoot.length() - 1) != '/') {
            mTileCacheRoot = mTileCacheRoot + "/";
        }
        mManagedTiles = new CachedLinkedHashSet<>(size);
        mPattern = pattern;
        mExtension = extension;
        traverseTiles();
    }

    public TileCache(String tileCacheRoot, String extension, int size) {
        this(tileCacheRoot, FILE_PATTERN, extension, size);
    }

    /**
     * @param id the id of tile to test.
     * @return true if tile (x, y, z) is contained in cache.
     */
    public boolean contains(Tile.Id id) {
        return mManagedTiles.contains(id);
    }

    /**
     * Refresh the tile cache for the given tile id.
     *
     * @param id identifies the tile to refresh
     */
    public void moveLast(Tile.Id id) {
        mManagedTiles.offer(id);
    }

    /**
     * Adds the given tile to the cache.
     *
     * @param data the tile data to save as a file
     * @return true, if successful
     */
    public boolean put(Tile.Id id, byte[] data) {
        int x = id.x;
        int y = id.y;
        int z = id.z;

        File file = new File(getTilePath(x, y, z));

        if (!file.exists()) {
            file.getParentFile().mkdirs();
            try {
                file.createNewFile();
            } catch (IOException e) {
                Log.e(TAG, "File " + file.getPath() + " not found");
                return false;
            }
        }

        OutputStream os = null;
        try {
            os = new BufferedOutputStream(new FileOutputStream(file));
            os.write(data);
            os.flush();
        } catch (FileNotFoundException e) {
            Log.e(TAG, "File " + file.getPath() + " not found");
            return false;
        } catch (IOException e) {
            Log.e(TAG, "Error while writing " + file.getPath());
            return false;
        } finally {
            if (os != null) {
                try {
                    os.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        if (mManagedTiles.isFull()) {
            Tile.Id oldestTile = mManagedTiles.poll();
            deleteTileFile(oldestTile);
        }
        return mManagedTiles.offer(id);
    }

    /**
     * Retrieve the Tile Cached File
     * @param id the tile id
     * @return the cached tile file
     */
    public File get(Tile.Id id) {
        if (!mManagedTiles.contains(id)) {
            throw new IllegalArgumentException("Cannot retrieve tile from cache.");
        }
        File f = new File(getTilePath(id.x, id.y, id.z));
        return f;
    }

    /**
     * Retrieve the Tile Cached File
     * @param id the tile id
     * @return the cached tile file
     */
    public T get(Tile.Id id, Mapper<T, InputStream> mapper) {
        if (!mManagedTiles.contains(id)) {
            throw new IllegalArgumentException("Cannot retrieve tile from cache.");
        }
        File f = new File(getTilePath(id.x, id.y, id.z));
        try {
            return mapper.convert(new FileInputStream(f));
        } catch (FileNotFoundException e) {
            Log.e(TAG, "Error retrieving file from cache: " + e.getMessage());
        }
        return null;
    }




    private String getTilePath(int x, int y, int z) {
        return mTileCacheRoot + z + "/" + x + "/" + y + "." + mExtension;
    }

    private void deleteTileFile(Tile.Id id) {
        File file = new File(getTilePath(id.x, id.y, id.z));
        file.delete();
    }

    private void traverseTiles() {
        traverseTiles(new File(mTileCacheRoot));
    }

    private void traverseTiles(File dir) {
        if (dir.exists()) {
            File[] files = dir.listFiles();
            for (File file : files) {
                if (file.isDirectory()) {
                    traverseTiles(file);
                } else {
                    String filename = file.getPath();
                    Matcher m = mPattern.matcher(filename);
                    if (m.find()) {
                        int z = Integer.valueOf(m.group(1));
                        int x = Integer.valueOf(m.group(2));
                        int y = Integer.valueOf(m.group(3));
                        Tile.Id id = new Tile.Id(x, y, z);
                        if (mManagedTiles.isFull()) {
                            Tile.Id oldestTile = mManagedTiles.poll();
                            deleteTileFile(oldestTile);
                        }
                        mManagedTiles.offer(id);
                    }
                }
            }
        }
    }

}
