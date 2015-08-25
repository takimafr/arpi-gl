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
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import mobi.designmyapp.arpigl.model.Tile;

public final class TileCache {

    private static final String TAG = TileCache.class.getSimpleName();
    private static final Pattern FILE_PATTERN = Pattern.compile("(\\d+)/(\\d+)/(\\d+)\\.png$");

    private String mDirPath;
    private int mSize;
    private CachedLinkedHashSet<Tile.Id> mManagedTiles;

    public TileCache(String dirPath, int size) {
        mDirPath = dirPath;
        if (mDirPath.charAt(mDirPath.length() - 1) != '/') {
            mDirPath = mDirPath + "/";
        }
        mSize = size;
        mManagedTiles = new CachedLinkedHashSet<>(mSize);
        mTraverseTiles();
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
    public void refresh(Tile.Id id) {
        mManagedTiles.refresh(id);
    }

    /**
     * Adds the given tile to the cache.
     *
     * @param tile the tile to add
     * @return true, if successful
     */
    public boolean add(Tile tile) {
        Tile.Id id = tile.getId();
        int x = id.x;
        int y = id.y;
        int z = id.z;
        byte[] data = tile.getData();

        File file = new File(mGetTilePath(x, y, z));

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
            e.printStackTrace();
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

        Tile.Id removedId = mManagedTiles.push(new Tile.Id(x, y, z));
        if (removedId != null) {
            deleteTileFile(removedId);
            mShrink();
        }

        return true;

    }


    private String mGetTilePath(int x, int y, int z) {
        return mDirPath + z + "/" + x + "/" + y + ".png";
    }

    private void mShrink() {
        List<Tile.Id> deleted = mManagedTiles.shrink(mSize - mSize / 4);
        for (Tile.Id id : deleted) {
            deleteTileFile(id);
        }
    }

    private void deleteTileFile(Tile.Id id) {
        File file = new File(mGetTilePath(id.x, id.y, id.z));
        file.delete();
    }

    private void mTraverseTiles() {
        mTraverseTiles(new File(mDirPath));
    }

    private void mTraverseTiles(File dir) {
        if (dir.exists()) {
            File[] files = dir.listFiles();
            for (File file : files) {
                if (file.isDirectory()) {
                    mTraverseTiles(file);
                } else {
                    String filename = file.getPath();
                    Matcher m = FILE_PATTERN.matcher(filename);
                    if (m.find()) {
                        int z = Integer.valueOf(m.group(1));
                        int x = Integer.valueOf(m.group(2));
                        int y = Integer.valueOf(m.group(3));
                        Tile.Id removed = mManagedTiles.push(new Tile.Id(x, y, z));
                        if (removed != null) {
                            deleteTileFile(removed);
                        }
                    }
                }
            }
        }
    }

}
