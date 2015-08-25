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

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Locale;

import mobi.designmyapp.arpigl.model.Tile;

/**
 * Contains various utilities.
 */
public final class ProjectionUtils {

    private static final double EQUATOR_LENGTH_IN_METERS = 40075016.686;

    public static final DecimalFormat COORDINATES_FORMAT;
    static {
        COORDINATES_FORMAT = new DecimalFormat("##.####################", DecimalFormatSymbols.getInstance(Locale.US));
    }

    /**
     * Default Constructor
     */
    private ProjectionUtils() {
    }

    /**
     * Convert longitude to tile X coord format, for the given zoom level.
     *
     * @param lng  the longitude
     * @param zoom the zoom level
     * @return the the tile X coord.
     * @see http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
     */
    public static int lng2tilex(double lng, int zoom) {
        return (int) Math.floor((lng + 180) / 360 * (1 << zoom));
    }

    /**
     * Convert latitude to tile X coord format, for the given zoom level.
     *
     * @param lat  the latitude
     * @param zoom the zoom level
     * @return the the tile X coord.
     * @see http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
     */
    public static int lat2tiley(double lat, int zoom) {
        return (int) Math.floor((1 - Math.log(Math.tan(Math.toRadians(lat)) + 1 / Math.cos(Math.toRadians(lat)))
                / Math.PI)
                / 2 * (1 << zoom));
    }

    public static double tile2lon(int x, int z) {
        return x / Math.pow(2.0, z) * 360.0 - 180;
    }

    public static double tile2lat(int y, int z) {
        double n = Math.PI - (2.0 * Math.PI * y) / Math.pow(2.0, z);
        return Math.toDegrees(Math.atan(Math.sinh(n)));
    }

    public static double[] tile2latLon(int x, int y, int z) {
        return new double[]{tile2lat(y, z), tile2lon(x, z)};
    }

    public static double[] tile2latLon(Tile.Id tile) {
        return tile2latLon(tile.x, tile.y, tile.z);
    }

    public static double tileSizeInMeters(Tile.Id tile, int zoom) {
        double tileSizeForZoomAtEquator = EQUATOR_LENGTH_IN_METERS / Math.pow(2, zoom);
        double lat = tile2lat(tile.y, tile.z);
        return tileSizeForZoomAtEquator * Math.cos(Math.toRadians(lat));
    }

    public static double circumscribedCircleRadiusInMeters(Tile.Id tile, int zoom) {
        double tileSize = tileSizeInMeters(tile, zoom);
        return tileSize / Math.sqrt(2);
    }

}
