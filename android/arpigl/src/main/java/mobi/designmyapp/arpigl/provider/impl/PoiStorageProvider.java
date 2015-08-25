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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.HashSet;
import java.util.Set;

import mobi.designmyapp.arpigl.event.PoiEvent;
import mobi.designmyapp.arpigl.exception.PoiMapperException;
import mobi.designmyapp.arpigl.mapper.DefaultInputStreamPoiMapper;
import mobi.designmyapp.arpigl.mapper.PoiMapper;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.PoiProvider;

public class PoiStorageProvider extends PoiProvider<InputStream> {

    private static final int OFFSET = 2;

    private File mPath;

    /**
     * Instantiates a new poi storage provider. This provider relies on POI
     * description installed on the storage, at installation time.
     */
    public PoiStorageProvider(File path) {
        this(path, DefaultInputStreamPoiMapper.class);
    }

    /**
     * Instantiates a new poi storage provider. This provider relies on POI
     * description installed on the storage, at installation time.
     *
     * @param path root folder where poi are stored by tiles
     */
    public PoiStorageProvider(File path, Class<? extends PoiMapper<InputStream>> mapperClass) {
        super(path.getAbsolutePath(), mapperClass);
        mPath = path;
    }


    @Override
    public final void fetch(Tile.Id tileId) {

        int x0 = tileId.x;
        int y0 = tileId.y;
        int z = tileId.z;

        for (int x = x0 - OFFSET; x <= x0 + OFFSET; ++x) {
            for (int y = y0 - OFFSET; y <= y0 + OFFSET; ++y) {
                // Check for 'localized pois'.
                Set<Poi> pois = new HashSet<>();
                String fileName = "" + z + "/" + x + "/" + y + ".json";
                File poisFile = new File(mPath, fileName);
                if (poisFile.exists()) {
                    FileInputStream fis;
                    try {
                        fis = new FileInputStream(poisFile);
                        pois = convert(fis);

                    } catch (FileNotFoundException | PoiMapperException e) {
                        throw new RuntimeException(e);
                    }
                }
                postEvent(new PoiEvent(pois));
            }
        }

    }

}
