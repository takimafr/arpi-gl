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

package mobi.designmyapp.arpigl.engine;


import mobi.designmyapp.arpigl.model.Poi;

/**
 * List of action to perform on the engine. Provide the same (or equivalent)
 * interface of native class dma::geo::GeoEngine.
 *
 * @author Nicolas THIERION.
 * @version 0.2.3 - Changelog : removed exit codes in return values.
 */
public interface EngineController {

    /**
     * Add a new Point of interest inside the scene, at the location defined for
     * this poi.
     *
     * @param poi poi to add
     */
    void addPoi(Poi poi);

    /**
     * Remove the given poi from the scene. You should also consider to call
     * PoiFactory.deletePoi().
     *
     * @param poi the poi to remove.
     * @throws IllegalStateException if called while the engine is not initialized.
     */
    void removePoi(Poi poi) throws IllegalStateException;


    /**
     * Set the position for the poi represented by the provided sid.
     *
     * @param sid the poi sid
     * @param lat the latitude
     * @param lng the longitude
     * @param alt the altitude
     */
    void setPoiPosition(String sid, double lat, double lng, double alt);


    /**
     * Set the provided color for the poi represented be the sid.
     *
     * @param sid   the poi sid
     * @param color the color to set
     */
    void setPoiColor(String sid, int color);

    /**
     * Set the position of the camera.
     *
     * @param lat camera latitude
     * @param lon camera longitude
     * @param alt camera altitude
     */
    void setCameraPosition(double lat, double lon, double alt);

    /**
     * Set the position of the camera. Keep the old value of altitude
     *
     * @param lat camera latitude
     * @param lon camera longitude
     */
    void setCameraPosition(double lat, double lon);

    /**
     * Sets the origin of the scene. Must be called one to put Origin near
     * current camera position.
     *
     * @param latitude  origin latitude
     * @param longitude origin longitude
     */
    void setOrigin(double latitude, double longitude);


    /**
     * Set the skybox corresponding to the provided sid.
     *
     * @param sid the skybox sid
     */
    void setSkyBox(String sid);


    /**
     * Enable/Disable the skybox.
     *
     * @param enabled if the skybox is enabled
     */
    void setSkyBoxEnabled(boolean enabled);

    /**
     * Modifying the returned value doesn't make any change on actual camera
     * rotation. You should call {@link #setCameraRotation(float[])} to make the
     * changes to take effect.
     *
     * @return a copy of the current rotation matrix of the 3D scene.
     */
    float[] getCameraRotationMatrix();

    /**
     * set the rotation matrix to apply a rotation on the 3D scene.
     *
     * @param rotationMatrix the rotation matrix.
     */
    void setCameraRotation(float[] rotationMatrix);

    /**
     * Checks if is installed.
     *
     * @return true if the engine is installed.
     */
    boolean isInstalled();

}