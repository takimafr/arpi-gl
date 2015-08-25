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

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;

import mobi.designmyapp.arpigl.model.Poi;

import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.ALTITUDE;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.COLOR;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.ICON_ID;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.LATITUDE;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.LONGITUDE;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.POI_ARRAY;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.POI_ID;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.SHAPE_ID;

/**
 * Created by Loïc Ortola on 06/08/2015.
 * Returns a JSONObject representation of a Poi object
 */
public class DefaultPoiJsonMapper implements Mapper<JSONObject, List<Poi>> {

    @Override
    public JSONObject convert(List<Poi> source) {
        JSONObject root = new JSONObject();

        try {
            JSONArray jsa = new JSONArray();

            for (Poi poi : source) {
                JSONObject jso = new JSONObject();
                jso.put(POI_ID, poi.getId());
                jso.put(SHAPE_ID, poi.getShapeId());
                jso.put(LATITUDE, poi.getLatitude());
                jso.put(LONGITUDE, poi.getLongitude());
                jso.put(ALTITUDE, poi.getAltitude());
                jso.put(COLOR, poi.getColor());
                if (poi.hasIcon()) {
                    jso.put(ICON_ID, poi.getIconId());
                }
                jsa.put(jso);
            }

            root.put(POI_ARRAY, jsa);
        } catch (JSONException e) {
            throw new IllegalStateException(e);
        }

        return root;
    }
}
