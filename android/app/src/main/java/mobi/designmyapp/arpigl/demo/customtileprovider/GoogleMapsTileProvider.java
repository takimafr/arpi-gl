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
package mobi.designmyapp.arpigl.demo.customtileprovider;

import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.impl.NetworkTileProvider;
import mobi.designmyapp.arpigl.util.ProjectionUtils;

/**
 * Created by Loïc Ortola on 23/08/2015.
 */
public class GoogleMapsTileProvider extends NetworkTileProvider {

    private static final String GOOGLE_MAPS_ENDPOINT = "https://maps.googleapis.com/maps/api/staticmap?center={lat},{lon}&zoom={z}&size=256x256&key={key}";

    public GoogleMapsTileProvider(final String apiKey) {
        super(GOOGLE_MAPS_ENDPOINT);

        setUriParser(new UriParser() {
            @Override
            public String parse(Tile.Id tile, String uri) {
                // OpenStreetMap Takes tile coordinates from their upper left corner.
                // Google maps Requests to have the tile-center instead. Therefore, we compute the middle delta with the next tile.
                // Wont work for upper tile limits
                double[] coords1 = ProjectionUtils.tile2latLon(tile);
                double[] coords2 = ProjectionUtils.tile2latLon(tile.x + 1, tile.y + 1, tile.z);
                double[] coords = {(coords1[0] + coords2[0]) / 2, (coords1[1] + coords2[1]) / 2};

                return uri.replace("{lat}", ProjectionUtils.COORDINATES_FORMAT.format(coords[0]))
                        .replace("{lon}", ProjectionUtils.COORDINATES_FORMAT.format(coords[1]))
                        .replace("{z}", String.valueOf(tile.z))
                        .replace("{key}", apiKey);
            }
        });
    }
}
