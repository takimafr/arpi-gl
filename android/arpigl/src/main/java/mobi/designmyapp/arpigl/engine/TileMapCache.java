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

package mobi.designmyapp.arpigl.engine;

import java.util.HashSet;
import java.util.Set;

import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.util.ProjectionUtils;
import mobi.designmyapp.arpigl.util.TileCache;

public class TileMapCache {

    private static int DEFAULT_TILE_MAP_SIZE = 5;
    /* ***
     * ATTRIBUTES
     */

    private final TileCache mTileCache;
    private int mTileMapSize;

    public TileMapCache(String cacheDir, int cacheSize) {
        this(cacheDir, cacheSize, DEFAULT_TILE_MAP_SIZE);
    }

    public TileMapCache(String cacheDir, int cacheSize, int tileMapSize) {
        mTileCache = new TileCache(cacheDir, cacheSize);
        mTileMapSize = tileMapSize;

    }

    public void updateCache(int x, int y, int z) {
        // we just received a new position : update tile cache.
        Set<Tile.Id> newTileIds = getTileIdsInRange(x, y, z);
        for (Tile.Id id : newTileIds) {
            if (mTileCache.contains(id)) {
                mTileCache.refresh(id);
            }
        }
    }

    private Set<Tile.Id> getTileIdsInRange(int x0, int y0, int z0) {
        int offset = (mTileMapSize / 2);
        Set<Tile.Id> res = new HashSet<Tile.Id>();
        for (int x = x0 - offset; x <= x0 + offset; ++x) {
            for (int y = y0 - offset; y <= y0 + offset; ++y) {
                res.add(new Tile.Id(x, y, z0));
            }
        }

        return res;
    }

    public final void add(Tile tile) {
        mTileCache.add(tile);
    }

    public final Set<Tile.Id> getTileIdsFor(double latitude, double longitude, int zoom) {
        int y = ProjectionUtils.lat2tiley(latitude, zoom);
        int x = ProjectionUtils.lng2tilex(longitude, zoom);

        return getTileIdsInRange(x, y, zoom);
    }
}
