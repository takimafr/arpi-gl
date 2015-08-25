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

package mobi.designmyapp.arpigl.mapper;

import org.json.JSONException;

import java.io.IOException;
import java.io.InputStream;
import java.util.LinkedList;
import java.util.List;

import mobi.designmyapp.arpigl.exception.PoiMapperException;
import mobi.designmyapp.arpigl.model.Poi;

/**
 * A DefaultInputStreamPoiMapper maps a JSON description file to a list of
 * {@link Poi}.
 *
 * @author Nicolas THIERION
 */
public class DefaultInputStreamPoiMapper implements PoiMapper<InputStream> {

    /**
     * Creates a new {@link DefaultInputStreamPoiMapper}.
     */
    public DefaultInputStreamPoiMapper() {
    }

    /**
     * @throws PoiMapperException if an {@link IOException} or a {@link JSONException} occurs.
     *                            This happens if the given inputStream is not a well formatted
     *                            JSON, or if the given inputStream could not be opened.
     */
    @Override
    public final List<Poi> convert(InputStream jsonFile) {

        final DefaultInputStreamPoiParser parser = new DefaultInputStreamPoiParser(jsonFile);

        final LinkedList<Poi> res = new LinkedList<>();
        try {
            while (parser.hasNext()) {
                res.add(parser.next());
            }
        } finally {
            parser.close();
        }

        return res;
    }
}
