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

package mobi.designmyapp.arpigl.demo.custompoiproviders;

import android.graphics.Color;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.util.LinkedList;
import java.util.List;

import mobi.designmyapp.arpigl.mapper.PoiMapper;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Shape;
import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.impl.NetworkPoiProvider;
import mobi.designmyapp.arpigl.util.IOUtils;
import mobi.designmyapp.arpigl.util.ProjectionUtils;

/**
 * Created by Loïc Ortola.
 * This sample class implements a custom poi data provider from the famous OpenDataSoft Service.
 * You can implement your own provider very easily by extending the PoiNetworkProvider class.
 */
public class CustomGooglePlacesPoiProvider extends NetworkPoiProvider {

    public static final String FETCH_URL_PATTERN = "https://maps.googleapis.com/maps/api/place/nearbysearch/json?location={lat},{lon}&radius={radius}&types=food&key={key}";
    public static final double POI_ALTITUDE = 5.0;                                                                                                                                                                                                                 // in

    public CustomGooglePlacesPoiProvider(final String apiKey) {
        super(FETCH_URL_PATTERN, GoogleMapsPlacesPoiProviderMapper.class);

        setUriParser(new UriParser() {
            @Override
            public String parse(Tile.Id tile, String uri) {
                double[] coords1 = ProjectionUtils.tile2latLon(tile);
                double[] coords2 = ProjectionUtils.tile2latLon(tile.x + 1, tile.y + 1, tile.z);
                double[] coords = {(coords1[0] + coords2[0]) / 2, (coords1[1] + coords2[1]) / 2};

                // As we have 5x5 tiles, the radius of the center tile will be multiplied by 5.
                double radius = 5 * ProjectionUtils.circumscribedCircleRadiusInMeters(tile, 19);

                return uri.replace("{lat}", ProjectionUtils.COORDINATES_FORMAT.format(coords[0]))
                        .replace("{lon}", ProjectionUtils.COORDINATES_FORMAT.format(coords[1]))
                        .replace("{radius}", String.valueOf(radius))
                        .replace("{key}", apiKey);
            }
        });
    }

    private static String getShapeForIcon(String icon) {
        if (icon.equals("cafe")) {
            return Shape.Default.BALLOON.getSid();
        }
        return Shape.Default.PYRAMID.getSid();
    }

    private static int getColorForType(String label) {
        if (label.contains("cafe")) {
            return Color.BLUE;
        }
        return Color.RED;
    }

    private static String getIconForType(String label) {
        if (label.contains("cafe")) {
            return "coffee";
        }
        return "restaurant";
    }

    /**
     * This custom mapper will adapt the OpenDataSoft Poi data model to the ArpiGl-compliant Poi data model.
     */
    public static class GoogleMapsPlacesPoiProviderMapper implements PoiMapper<InputStream> {


        private static final String ENCODING = "UTF-8";

        private JSONArray mPoiArrays;

        private int mCount;

        @Override
        public List<Poi> convert(InputStream jsonFile) {
            List<Poi> res = new LinkedList<>();

            try {
                JSONObject jsonObject = IOUtils.toJsonObject(jsonFile, ENCODING);
                mPoiArrays = jsonObject.getJSONArray("results");
                mCount = mPoiArrays.length();

                for (int i = 0; i < mCount; ++i) {
                    JSONObject jso = mPoiArrays.getJSONObject(i);

                    Poi.Builder builder = new Poi.Builder();

                    String types = jso.getJSONArray("types").toString();

                    JSONObject location = jso.getJSONObject("geometry").getJSONObject("location");

                    double latitude = location.getDouble("lat");
                    double longitude = location.getDouble("lng");

                    String poiId = jso.getString("id");

                    String icon = getIconForType(types);
                    int color = getColorForType(types);
                    String shape = getShapeForIcon(icon);

                    // build the POI
                    builder.shape(shape);
                    builder.icon(icon);
                    builder.color(color);
                    builder.latitude(latitude);
                    builder.longitude(longitude);
                    builder.altitude(POI_ALTITUDE);
                    builder.id(poiId);
                    res.add(builder.build());
                }

                return res;
            } catch (JSONException | IOException e) {
                throw new IllegalStateException(e);
            }
        }

    }

}
