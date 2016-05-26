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

import android.graphics.Color;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.ParseException;
import java.util.Iterator;

import mobi.designmyapp.arpigl.model.Poi;

import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.ALTITUDE;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.COLOR;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.ICON_ID;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.LATITUDE;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.LONGITUDE;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.POI_ARRAY;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.POI_ID;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.SHAPE_ID;
import static mobi.designmyapp.arpigl.mapper.PoiMapper.DefaultPoiStructure.ANIMATED;

/**
 * Helper class to create a new Poi by parsing a JSON file.
 *
 * @author Nicolas THIERION
 */
final class DefaultInputStreamPoiParser implements Iterator<Poi> {

    private static final String TAG = DefaultInputStreamPoiParser.class.getSimpleName();

    /* ***
     * ATTRIBUTES
     */
    /**
     * Inpustream to read.
     */
    private final InputStream mInputStream;
    /**
     * if mIsArray, should contains all Poi JsonObjects of the array.
     */
    private JSONArray mPoiArrays;
    /**
     * if !mIsArray, this holds the unique parsable POI.
     */
    private JSONObject mJsonObject;
    /**
     * count of total available POIs in the JSON.
     */
    private int mCount;
    /**
     * current POI index.
     */
    private int mCurrent;

    /**
     * Create a new PoiJsonParser that will read the given {@code InputStream}.
     *
     * @param jsonFile input to read
     */
    public DefaultInputStreamPoiParser(InputStream jsonFile) {
        mInputStream = jsonFile;

    }

    /**
     * Open & read the provided inputStream.
     *
     * @throws JSONException if the given inputStream is not a well formatted JSON
     * @throws IOException   if the given inputStream could not be opened.
     */
    private void read() {
        if (isOpen()) {
            throw new IllegalStateException("stream already opened.");
        }
        BufferedReader br = null;
        try {
            br = new BufferedReader(new InputStreamReader(mInputStream, PoiMapper.DEFAULT_ENCODING));

            final StringBuilder sb = new StringBuilder();

            String inputStr;
            while ((inputStr = br.readLine()) != null) {
                sb.append(inputStr);
            }
            mJsonObject = new JSONObject(sb.toString());
                mPoiArrays = mJsonObject.getJSONArray(POI_ARRAY);
                mCount = mPoiArrays.length();

        } catch (IOException | JSONException e) {
            throw new IllegalStateException(e);
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    throw new IllegalStateException(e);
                }
            }
        }
    }

    /**
     * @return true if the stream has been opened.
     */
    private boolean isOpen() {
        return mJsonObject != null;
    }

    /**
     * Call {@code open()} if not opened.
     *
     * @return true if there is still some entry to load.
     * @throws JSONException if the given inputStream is not a well formatted JSON
     * @throws IOException   if the given inputStream could not be opened.
     */
    @Override
    public synchronized boolean hasNext() {
        if (!isOpen()) {
            read();
        }
        return mCurrent < mCount;
    }

    /**
     * Parse the next entry.
     *
     * @throws JSONException if the given inputStream is not a well formatted JSON
     */
    @Override
    public synchronized Poi next() {
        if (!isOpen()) {
            read();
        }
        if (!(mCurrent < mCount)) {
            Log.w(TAG, "Error while parsing POI: EOF");
            throw new IllegalStateException("No more objects to read.");
        }
        try {
            Poi result = null;

                result = parsePoi(mPoiArrays.getJSONObject(mCurrent));
            ++mCurrent;
            return result;
        } catch (JSONException e) {
            Log.w(TAG, "Error while parsing POI: " + e.getMessage());
            throw new IllegalStateException(e);
        }
    }

    @Override
    public void remove() {
        throw new UnsupportedOperationException();
    }

    /* ***
     * PRIVATE METHODS
     */

    private Poi parsePoi(JSONObject jso) throws JSONException {
        double latitude = jso.getDouble(LATITUDE);
        double longitude = jso.getDouble(LONGITUDE);
        String meshId = jso.getString(SHAPE_ID);
        String poiId = jso.getString(POI_ID);
        boolean animated = true;
        if (jso.has(ANIMATED)) {
            animated = jso.getBoolean(ANIMATED);
        }

        String iconId = null;
        if (jso.has(ICON_ID)) {
            iconId = jso.getString(ICON_ID);
        }

        double altitude = 0.0;
        if (jso.has(ALTITUDE)) {
            altitude = jso.getDouble(ALTITUDE);
        }

        int color = Color.MAGENTA;
        if (jso.has(COLOR)) {
            try {
                String colorStr = jso.getString(COLOR);
                if (colorStr.startsWith("#")) {
                    color = Color.parseColor(colorStr.toUpperCase());
                } else {
                    color = Integer.parseInt(colorStr, 16);
                }
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
        }

        //TODO remove
        switch (meshId) {
            case "cab":
                color = Color.GRAY;
                animated = false;
                break;
            case "defibrillator":
                color = Color.parseColor("#F22C2C");
                break;
            default:
                color = Color.parseColor("darkgray");
                break;
        }

        return new Poi(poiId, meshId, iconId, color, latitude, longitude, altitude, animated);
    }

    public void close() {
        if (mInputStream != null) {
            try {
                mInputStream.close();
            } catch (IOException e) {
                Log.w(TAG, "Error while closing stream: " + e.getMessage());
                throw new IllegalStateException(e);
            }
        }
    }

}
