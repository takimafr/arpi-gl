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

import java.util.List;

import mobi.designmyapp.arpigl.model.Poi;

/**
 * A {@link PoiMapper} can convert a source of certain type to a list of
 * {@link Poi}.
 *
 * @param <T> the datasource type.
 * @author Nicolas THIERION.
 */
public interface PoiMapper<T> extends Mapper<List<Poi>, T> {

     /* ***
     * JSON KEYS
     */

    public static final String DEFAULT_ENCODING = "UTF-8";

    /**
     * @param source Source describing a list of {@link Poi}
     * @return the list of {@link Poi} that matches the Pois
     */
    List<Poi> convert(T source);

    /**
     * POI default Structure.
     * POIs : [
     * {
     * "shape": ""
     * "icon": ""
     * "lat": 11.00
     * "lon": 11.00
     * "alt": 11.00
     * "zoom": 19
     * "sid": ""
     * "color": ""
     * }
     * ]
     *
     * @author Nicolas THIERION.
     */
    public static class DefaultPoiStructure {
        /**
         * Root.
         */
        public static final String POI_ARRAY = "POIs";
        /**
         * The shape key.
         */
        public static final String SHAPE_ID = "shape";
        /**
         * The material key.
         */
        public static final String ICON_ID = "icon";
        /**
         * The latitude key.
         */
        public static final String LATITUDE = "lat";
        /**
         * The longitude key.
         */
        public static final String LONGITUDE = "lon";
        /**
         * The altitude key.
         */
        public static final String ALTITUDE = "alt";
        /**
         * The poi id key.
         */
        public static final String POI_ID = "sid";
        /**
         * The poi color.
         */
        public static final String COLOR = "color";
    }


}
