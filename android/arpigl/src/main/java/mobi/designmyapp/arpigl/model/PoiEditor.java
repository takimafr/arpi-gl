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

package mobi.designmyapp.arpigl.model;

import mobi.designmyapp.arpigl.engine.EngineController;

/**
 * Created by Loïc Ortola on 10/08/2015.
 */
class PoiEditor {

    private EngineController mEngineController;
    private Poi poi;
    private Poi.Builder tmpPoi;

    public PoiEditor(Poi poi, EngineController engineController) {
        this.poi = poi;
        mEngineController = engineController;
        tmpPoi = new Poi.Builder().fromPoi(poi);
    }

    /**
     * Sets the position of this poi.
     *
     * @param latitude  the latitude
     * @param longitude the longitude
     * @param altitude  the altitude
     * @return the current editor
     */
    public PoiEditor position(double latitude, double longitude, double altitude) {
        tmpPoi.latitude(latitude);
        tmpPoi.longitude(longitude);
        tmpPoi.altitude(altitude);
        return this;
    }

    /**
     * Sets the position of this poi.
     *
     * @param latitude  the latitude
     * @param longitude the longitude
     * @return the current editor
     */
    public PoiEditor position(double latitude, double longitude) {
        tmpPoi.latitude(latitude);
        tmpPoi.longitude(longitude);
        return this;
    }

    /**
     * Sets the position of this poi.
     *
     * @param pos [lat, lng] or [lat, lng, alt]
     * @return the current editor
     */
    public PoiEditor position(double[] pos) {
        tmpPoi.latitude(pos[0]);
        tmpPoi.longitude(pos[1]);
        if (pos.length == 3) {
            tmpPoi.altitude(pos[2]);
        }
        return this;
    }

    /**
     * Sets the latitude of the poi.
     *
     * @param latitude the new latitude
     * @return the current editor
     */
    public PoiEditor latitude(double latitude) {
        tmpPoi.latitude(latitude);
        return this;
    }

    /**
     * Sets the longitude of the poi.
     *
     * @param longitude the new longitude
     * @return the current editor
     */
    public PoiEditor longitude(double longitude) {
        tmpPoi.longitude(longitude);
        return this;
    }

    /**
     * Sets the altitude of the poi.
     *
     * @param altitude the new altitude
     * @return the current editor
     */
    public PoiEditor altitude(double altitude) {
        tmpPoi.altitude(altitude);
        return this;
    }

    /**
     * Sets the color of the poi.
     *
     * @param color the color
     * @return the current editor
     */
    public PoiEditor color(int color) {
        tmpPoi.color(color);
        return this;
    }

    /**
     * Commit changes to POI instance and to Map GL Engine.
     */
    public void commit() {
        commitPoi(poi, tmpPoi);
    }

    /**
     * Commit tmpPoi state to current poi instance and to Map GL Engine.
     *
     * @param poi    the current poi instance
     * @param tmpPoi the tmp poi state to commit
     */
    private void commitPoi(Poi poi, Poi.Builder tmpPoi) {
        poi.mShapeId = tmpPoi.mShapeId;
        poi.mPoiId = tmpPoi.mPoiId;
        poi.mIcon = tmpPoi.mIcon;
        poi.mColor = tmpPoi.mColor;
        poi.mLatitude = tmpPoi.mLatitude;
        poi.mLongitude = tmpPoi.mLongitude;
        poi.mAltitude = tmpPoi.mAltitude;
        mEngineController.setPoiPosition(tmpPoi.mPoiId, tmpPoi.mLatitude, tmpPoi.mLongitude, tmpPoi.mAltitude);
        mEngineController.setPoiColor(tmpPoi.mPoiId, tmpPoi.mColor);
    }
}
