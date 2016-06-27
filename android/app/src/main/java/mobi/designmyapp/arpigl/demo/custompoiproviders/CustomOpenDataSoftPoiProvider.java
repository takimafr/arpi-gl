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

import java.io.IOException;
import java.io.InputStream;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;

import mobi.designmyapp.arpigl.mapper.PoiMapper;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Shape;
import mobi.designmyapp.arpigl.provider.impl.NetworkPoiProvider;
import mobi.designmyapp.arpigl.util.IOUtils;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.graphics.Color;

/**
 * Created by Loïc Ortola.
 * This sample class implements a custom poi data provider from the famous OpenDataSoft Service.
 * You can implement your own provider very easily by extending the PoiNetworkProvider class.
 */
public class CustomOpenDataSoftPoiProvider extends NetworkPoiProvider {

    public static final String FETCH_URL_PATTERN = "http://public.opendatasoft.com/api/records/1.0/search?dataset=mobiliereaupotableparis2011&facet=info&facet=libelle&geofilter.distance={lat},{lon},{radius}&rows=1000";
    public static final double POI_ALTITUDE = 3.0;
    private static final int BLUE_WATER = 0xFF013ADF;
    private static final int GREEN_CLEAN = 0xFF088A4B;
    private static final int RED_HYDRANT = 0xFFB40404;

    public CustomOpenDataSoftPoiProvider() {
        super(FETCH_URL_PATTERN, OSMPoiProviderMapper.class);
    }

    private static String getShapeFromColor(int color) {
        switch (color) {
            case BLUE_WATER:
                return Shape.Default.BALLOON.getSid();
            case RED_HYDRANT:
                return "hydrant";
            case GREEN_CLEAN:
                return Shape.Default.PYRAMID.getSid();
            default:
                return Shape.Default.SPHERE.getSid();
        }
    }

    private static int getColorForLabelLike(String label) {
        label = label.toLowerCase(Locale.ENGLISH);
        // Bouche de lavage
        if (label.contains("bla")) {
            return GREEN_CLEAN;
        }
        // Vanne d'Eau ou fontaine
        if (label.contains("eau") || label.contains("fon") || label.contains("wal")) {
            return BLUE_WATER;
        }
        // Bouche d'incendie, d'arrosage
        if (label.contains("bino") || label.contains("bar") || label.contains("binb")) {
            return RED_HYDRANT;
        }
        return Color.GRAY;
    }

    private static String getIconFromColor(int color) {
        switch (color) {
            case BLUE_WATER:
                return "water-hose";
            case GREEN_CLEAN:
                return "sink";
            case RED_HYDRANT:
                return "hydrant-diffuse";
            default:
                return null;
        }
    }

    /**
     * This custom mapper will adapt the OpenDataSoft Poi data model to the ArpiGl-compliant Poi data model.
     */
    public static class OSMPoiProviderMapper implements PoiMapper<InputStream> {


        private static final String ENCODING = "UTF-8";

        private DecimalFormat precisionFormatter = new DecimalFormat("##.####", DecimalFormatSymbols.getInstance(Locale.US));
        private JSONArray mPoiArrays;

        private int mCount;

        @Override
        public List<Poi> convert(InputStream jsonFile) {
            List<Poi> res = new LinkedList<>();

            try {

                JSONObject jsonObject = IOUtils.toJsonObject(jsonFile, ENCODING);
                mPoiArrays = jsonObject.getJSONArray("records");
                mCount = mPoiArrays.length();

                for (int i = 0; i < mCount; ++i) {

                    JSONObject jso = mPoiArrays.getJSONObject(i);

                    JSONObject fields = jso.getJSONObject("fields");

                    JSONArray coordinateSets = fields.getJSONObject("geom").getJSONArray("coordinates");

                    if (coordinateSets.length() == 0) {
                        return null;
                    }

                    //Either we have a set of coordinates, or we can have an array of sets of coordinates.
                    boolean iterate = coordinateSets.getJSONArray(0).optJSONArray(0) != null;

                    if (iterate) {
                        for (int j = 0; j < coordinateSets.length(); j++) {
                            extractPoi(res, jso.getString("recordid"), fields.getString("info"), coordinateSets.getJSONArray(j));
                        }
                    } else {
                        extractPoi(res, jso.getString("recordid"), fields.getString("info"), coordinateSets);
                    }
                }

                return res;
            } catch (JSONException | IOException e) {
                throw new IllegalStateException(e);
            }
        }

        private void extractPoi(List<Poi> res, String recordId, String type, JSONArray coordinateSet) throws JSONException {
            for (int i = 0; i < coordinateSet.length(); i++) {
                    Poi.Builder builder = new Poi.Builder();

                    double longitude = coordinateSet.getJSONArray(i).getDouble(0);
                    double latitude = coordinateSet.getJSONArray(i).getDouble(1);

                    String poiId = precisionFormatter.format(latitude) + precisionFormatter.format(longitude);
                    int color = getColorForLabelLike(type);
                    String shape = getShapeFromColor(color);
                    String icon = getIconFromColor(color);

                    // build the POI
                    builder.shape(shape);
                    builder.icon(icon);
                    builder.color(color);
                    builder.longitude(longitude);
                    builder.latitude(latitude);
                    if (shape.equals("hydrant")) {
                        builder.altitude(0.1);
                    } else {
                        builder.altitude(POI_ALTITUDE);
                    }
                    builder.id(poiId);
                    Poi poi = builder.build();
                    if (!res.contains(poi)) {
                        res.add(poi);
                    }
            }
        }

    }

}
