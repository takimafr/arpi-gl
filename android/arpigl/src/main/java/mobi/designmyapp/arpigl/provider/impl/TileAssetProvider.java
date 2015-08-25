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

import android.content.Context;
import android.util.Log;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

import mobi.designmyapp.arpigl.event.TileEvent;
import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.TileProvider;
import mobi.designmyapp.arpigl.util.IOUtils;

public class TileAssetProvider extends TileProvider {

    private static final String TAG = TileAssetProvider.class.getSimpleName();

    private Context mContext;

    public TileAssetProvider(Context context, String path) {
        super(path);
        mContext = context;
    }

    @Override
    public final void fetch(Tile.Id tid) {
        int x = tid.x;
        int y = tid.y;
        int z = tid.z;

        String filepath = getUri() + "/" + z + "/" + x + "/" + y + ".png";
        try {
            InputStream is = mContext.getAssets().open(filepath);
            byte[] data = IOUtils.read(is);
            postEvent(new TileEvent(new Tile(data, tid)));
        } catch (FileNotFoundException e) {
            Log.e(TAG, "File " + filepath + " not found");
        } catch (IOException e) {
            Log.e(TAG, String.format("Error while fetching tile (%d, %d, %d)", x, y, z));
            e.printStackTrace();
        }
    }

}
