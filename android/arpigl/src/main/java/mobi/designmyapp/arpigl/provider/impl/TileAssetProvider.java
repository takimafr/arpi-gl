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

package mobi.designmyapp.arpigl.provider.impl;

import android.util.Log;

import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.TileProvider;

public class TileAssetProvider extends TileProvider {

    private static final String TAG = TileAssetProvider.class.getSimpleName();

    public TileAssetProvider(String path) {
        super(path);
    }

    @Override
    public final void fetch(Tile.Id tid) {
        // Nothing to do since tiles are installed at the right location, the engine will find them automatically
        // However if a tile is missing we can't do anything but log a message
        Log.d(TAG, String.format("Tile (%d, %d, %d) asked but not found", tid.x, tid.y, tid.z));
    }

    @Override
    public String getNamespace() {
        return mUri;
    }

}
