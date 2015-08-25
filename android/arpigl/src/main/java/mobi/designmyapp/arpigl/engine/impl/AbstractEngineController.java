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

package mobi.designmyapp.arpigl.engine.impl;


import mobi.designmyapp.arpigl.engine.Engine;
import mobi.designmyapp.arpigl.engine.EngineController;
import mobi.designmyapp.arpigl.model.Poi;

public abstract class AbstractEngineController implements EngineController {

    /* ***
     * ATTRIBUTES
     */
    private Engine mEngine;
    private double[] mCameraPosition = new double[3];

    /* ***
     * CONTROLLER
     */

    /**
     * Create a new engine controller to control the given {@link ArpiGlEngine}.
     *
     * @param engine the engine to control.
     */
    public AbstractEngineController(Engine engine) {
        mEngine = engine;
    }

    public double[] getCameraPosition() {
        return mCameraPosition;
    }

    /* ***
     * EngineController Overrides
     */

    @Override
    public final boolean isInstalled() {
        return mEngine.isInstalled();
    }

    @Override
    public final void addPoi(Poi poi) {
        mEngine.addPoi(poi);
    }

    @Override
    public final void removePoi(Poi poi) throws IllegalStateException {
        mEngine.removePoi(poi);
    }

    @Override
    public void setPoiPosition(String sid, double lat, double lng, double alt) {
        mEngine.setPoiPosition(sid, lat, lng, alt);
    }

    @Override
    public void setPoiColor(String sid, int color) {

    }

    @Override
    public void setCameraPosition(double lat, double lon, double alt) {
        mEngine.setCameraPosition(lat, lon, alt);
        mCameraPosition[0] = lat;
        mCameraPosition[1] = lon;
        mCameraPosition[2] = alt;
    }

    @Override
    public void setCameraPosition(double lat, double lon) {
        mEngine.setCameraPosition(lat, lon);
        mCameraPosition[0] = lat;
        mCameraPosition[1] = lon;
    }

    @Override
    public final void setOrigin(double latitude, double longitude) {
        mEngine.setOrigin(latitude, longitude);
    }

    @Override
    public void setSkyBox(String sid) {
        mEngine.setSkyBox(sid);
    }

    @Override
    public void setSkyBoxEnabled(boolean enabled) {
        mEngine.setSkyBoxEnabled(enabled);
    }

    @Override
    public final float[] getCameraRotationMatrix() {
        return mEngine.getCameraRotationMatrix();
    }

    @Override
    public final void setCameraRotation(float[] rotationMatrix) {
        mEngine.setCameraRotation(rotationMatrix);
    }

}
