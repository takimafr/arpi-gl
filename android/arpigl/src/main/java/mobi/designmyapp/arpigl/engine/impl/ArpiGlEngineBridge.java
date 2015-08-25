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
import mobi.designmyapp.arpigl.engine.EngineBridge;
import mobi.designmyapp.arpigl.listener.EngineListener;
import mobi.designmyapp.arpigl.model.Poi;
import android.graphics.Color;
import android.util.Log;

class ArpiGlEngineBridge implements EngineBridge {

    private static final String TAG = ArpiGlEngineBridge.class.getSimpleName();

    /**
     * Native address of the cpp object corresponding to this instance.
     */
    private long mNativeInstanceAddr;
    /**
     * where resource files are installed.
     */
    private String mInstallationDir;

    /* ***
     * CONSTRUCTORS
     */

    public ArpiGlEngineBridge(Engine engine) {
        mInstallationDir = engine.getInstallationDir();
        mNativeInstanceAddr = newEngine(mInstallationDir);
    }

    @Override
    public void destroy() {
        freeEngine(mNativeInstanceAddr);
    }

    /* ***
     * API PUBLIC METHODS
     */

    @Override
    public boolean init() {
        return init(mNativeInstanceAddr);
    }

    @Override
    public void refresh() {
        Log.v(TAG, "refresh...");
        refresh(mNativeInstanceAddr);
        Log.v(TAG, "refresh done");
    }

    @Override
    public void unload() {
        // Has to be called from Rendered thread
        unload(mNativeInstanceAddr);
    }

    @Override
    public void wipe() {
        // Has to be called from Rendered thread
        Log.v(TAG, "wipe...");
        wipe(mNativeInstanceAddr);
        Log.v(TAG, "wipe done");
    }

    @Override
    public void step() {
        step(mNativeInstanceAddr);
    }

    @Override
    public void addPoi(Poi poi) {
        int color = poi.getColor();

        final float maxShade = 255.0f;
        float r = (Color.red(color) / maxShade);
        float g = (Color.green(color) / maxShade);
        float b = (Color.blue(color) / maxShade);

        addPoi(mNativeInstanceAddr, poi.getId(), poi.getShapeId(), poi.getIconId(), r, g, b, poi.getLatitude(),
                poi.getLongitude(), poi.getAltitude());
    }

    @Override
    public void removePoi(Poi poi) {
        removePoi(mNativeInstanceAddr, poi.getId());
    }


    @Override
    public void setPoiPosition(String sid, double lat, double lng, double alt) {
        setPoiPosition(mNativeInstanceAddr, sid, lat, lng, alt);
    }

    @Override
    public void setPoiColor(String sid, int color) {
        final float maxShade = 255.0f;
        float r = (Color.red(color) / maxShade);
        float g = (Color.green(color) / maxShade);
        float b = (Color.blue(color) / maxShade);
        setPoiColor(mNativeInstanceAddr, sid, r, g, b);
    }

    @Override
    public boolean isInit() {
        return isInit(mNativeInstanceAddr);
    }

    @Override
    public boolean isAbleToDraw() {
        return isAbleToDraw(mNativeInstanceAddr);
    }

    @Override
    public boolean hasPoi(Poi poi) {
        return hasPoi(mNativeInstanceAddr, poi.getId());
    }

    @Override
    public void setSkyBox(String sid) {
        setSkyBox(mNativeInstanceAddr, sid);
    }

    @Override
    public void setSkyBoxEnabled(boolean enabled) {
        setSkyBoxEnabled(mNativeInstanceAddr, enabled);
    }

    @Override
    public void setSurfaceSize(int width, int height) {
        setSurfaceSize(mNativeInstanceAddr, width, height);
    }

    @Override
    public void setNativeCallbacks(EngineListener callbacks) {
        setCallbacks(mNativeInstanceAddr, ((AbstractEngineListener) callbacks).getNativeAddr());
    }

    @Override
    public void setCameraRotation(float[] rotationMatrix) {
        setCameraRotation(mNativeInstanceAddr, rotationMatrix);
    }

    @Override
    public void setCameraPosition(double lat, double lon) {
        setCameraPosition2d(mNativeInstanceAddr, lat, lon);
    }

    @Override
    public void setCameraPosition(double lat, double lon, double alt) {
        setCameraPosition3d(mNativeInstanceAddr, lat, lon, alt);
    }

    @Override
    public void setOrigin(double latitude, double longitude) {
        setOrigin(mNativeInstanceAddr, latitude, longitude);
    }

    @Override
    public boolean isInstalled() {
        return false;
    }


    @Override
    public void notifyTileAvailable(int x, int y, int z) {
        notifyTileAvailable(mNativeInstanceAddr, x, y, z);
    }

    @Override
    public float[] getCameraRotationMatrix() {
        throw new UnsupportedOperationException();
    }

    /* ***
     * NATIVE ROUTINES DEFINITION
     */

    private native long newEngine(String rootDir);

    private native long freeEngine(long nativeInstanceAddr);

    private native boolean init(long nativeInstanceAddr);

    private native void refresh(long nativeInstanceAddr);

    private native void unload(long nativeInstanceAddr);

    private native void wipe(long nativeInstanceAddr);

    private native void step(long nativeInstanceAddr);

    private native void addPoi(long nativeInstanceAddr, String sid, String shape, String icon, float r, float g,
                               float b, double lat, double lng, double alt);

    private native void removePoi(long nativeInstanceAddr, String sid);

    private native void setPoiPosition(long nativeInstanceAddr, String sid, double lat, double lng, double alt);

    private native void setPoiColor(long nativeInstanceAddr, String sid, float r, float g, float b);

    private native boolean isInit(long nativeInstanceAddr);

    private native boolean isAbleToDraw(long nativeInstanceAddr);

    private native boolean hasPoi(long nativeInstanceAddr, String sid);

    private native void setSkyBox(long nativeInstanceAddr, String sid);

    private native void setSkyBoxEnabled(long nativeInstanceAddr, boolean enabled);

    private native void setSurfaceSize(long nativeInstanceAddr, int width, int height);

    private native void setCallbacks(long nativeInstanceAddr, long nativeCallbackAddr);

    private native void setCameraRotation(long nativeInstanceAddr, float[] rotationMatrix);

    private native void setCameraPosition2d(long nativeInstanceAddr, double lat, double lon);

    private native void setCameraPosition3d(long nativeInstanceAddr, double lat, double lon, double alt);

    private native void setOrigin(long nativeInstanceAddr, double lat, double lon);

    private native void notifyTileAvailable(long nativeInstanceAddr, int x, int y, int z);
}