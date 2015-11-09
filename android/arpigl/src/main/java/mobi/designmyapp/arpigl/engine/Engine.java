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

import android.content.Context;
import android.graphics.Color;
import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.BuildConfig;
import mobi.designmyapp.arpigl.listener.EngineListener;
import mobi.designmyapp.arpigl.model.Poi;

/**
 * Draws an OpenGL scene to display some POIs on a map. Wraps native cpp class
 *
 * @author Nicolas THIERION
 */
public final class Engine implements Controller {

    /**
     * resource folder stored in assets, that must contain engine installation
     * files. Any file in this folder will be copied into the device's internal
     * storage.
     */
    public static final String RESOURCES_FOLDER = ArpiGlInstaller.INSTALLATION_DIR;
    /**
     * Debug tag.
     */
    private static final String TAG = Engine.class.getSimpleName();

    private static final double DEFAULT_CAMERA_ALTITUDE = 5.0;

    /**
     * Native .so library name.
     */
    private static final String JNI_LIB = "arpigl";

    // loads native library once.
    static {
        System.loadLibrary(JNI_LIB);
    }

    /* ***
     * ATTRIBUTES
     */
    /**
     * Native address of the cpp object corresponding to this instance.
     */
    private long mNativeInstanceAddr;
    /**
     * path of Engine's root dir where files are copied.
     */
    private final String mInstallationDir;
    /**
     * context of the application.
     */
    private final Context mContext;
    /**
     * current camera position.
     */
    private double[] mCameraPosition = new double[] {0.0, 0.0, DEFAULT_CAMERA_ALTITUDE};
    /**
     * Renderer for android's GLSurfaceView that uses this Engine as Rendering
     * engine.
     */
    private final Renderer mRenderer;

    /**
     * flag to tell if engine is installed.
     */
    private boolean mInstalled;
    /**
     * callbacks to be notified on native engine event. This callback is set by
     * default to discard events.
     */
    private NativeFallthroughEngineListener mNativeListener = new NativeFallthroughEngineListener();

    /* ***
     * CONSTRUCTOR
     */

    /**
     * Create a new MapGl Rendering engine. An engine is intended to be
     * construct by & only by the EngineGlFragment, that know how to manage its
     * lifecycle.
     *
     * @param context The context of the application, needed to get assets.
     */
    public Engine(Context context) {

        mContext = context;

        // set the installation path, where resource assets are (will be)
        // copied.
        final StringBuilder filesDirBuilder = new StringBuilder();
        filesDirBuilder
                .append(context.getFilesDir().getAbsolutePath())
                .append('/')
                .append(RESOURCES_FOLDER);
        mInstallationDir = filesDirBuilder.toString();

        mNativeInstanceAddr = newEngine(mInstallationDir);
        mRenderer = new Renderer(this);
        // register native callbacks
        setEngineListener(mNativeInstanceAddr, mNativeListener.getNativeAddr());

    }

    /* ***
     * LIFECYCLE CONTROL
     */

    @Override
    protected void finalize() throws Throwable {
        destroy();
        super.finalize();
    }

    /**
     * Destroys the engine, and free all memory on CPP side.
     */
    public void destroy() {
        freeEngine(mNativeInstanceAddr);
    }

    /**
     * Initialize the engine. Be careful to have a valid openGL context opened &
     * bound before calling this method. Otherwise, the method will fail. Be
     * careful to call {@link #setSurfaceSize(int, int)} to create a valid
     * drawing surface.
     *
     * @return true if initialized properly, false otherwise.
     */
    public boolean init() {
        throwIfNotInstalled("init()");
        return init(mNativeInstanceAddr);
    }

    /* ***
     * API METHODS
     */

    /**
     * Refreshes all OpenGL resources. The OpenGL context needs to be refreshed
     * after a call to {@link #wipe}. If the engine steps on a
     * not-refreshed context, exceptions will happen.
     */
    public void refresh() {
        throwIfNotInstalled("refresh()");
        if (isInit()) {
            refresh(mNativeInstanceAddr);
            updateTileDiffuseMaps(mNativeInstanceAddr);
        } else {
            Log.w(TAG, "refresh but engine is not init");
            if (BuildConfig.DEBUG) {
                throwIfNotInit("refresh()");
            }
        }
    }

    /**
     * Clean this engine.
     */
    public synchronized void unload() {
        // Has to be called from Rendered thread
        unload(mNativeInstanceAddr);
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
    public void removePoi(Poi poi) throws IllegalStateException {
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

    /**
     * Notify the engine that a tile is available.
     *
     * @param x the tile x
     * @param y the tile y
     * @param z the tile z
     */
    public void notifyTileAvailable(final int x, final int y, final int z) {
        notifyTileAvailable(mNativeInstanceAddr, x, y, z);
    }

    /**
     * Changes the zoom level
     * offset must be between [-1.0, 1.0]
     * if |offset| > 1.0f this will clamp the value to 1.0 -1.0
     * a positive offset will zoom in
     * a negative offset will zoom out
     */
    public void zoom(float offset) {
        zoom(mNativeInstanceAddr, offset);
    }

    /**
     * Selects the closest poi from the camera at the screen coordinate (x, y)
     * @param x the x screen coordinate
     * @param y the y screen coordinate
     */
    public void selectPoi(int x, int y) {
        selectPoi(mNativeInstanceAddr, x, y);
    }

    /* ***
     * GETTERS
     */

    public double[] getCameraPosition() {
        return mCameraPosition;
    }

    /**
     * Checks if is inits the.
     *
     * @return true if engine is initialized
     */
    public boolean isInit() {
        return isInit(mNativeInstanceAddr);
    }

    /**
     * Checks if is able to draw.
     *
     * @return true if the Engine is able to draw (it is initialized, and has a
     * valid openGL context.)
     */
    public boolean isAbleToDraw() {
        return isAbleToDraw(mNativeInstanceAddr);
    }

    /**
     * Checks for poi.
     *
     * @param poi the poi
     * @return true if the entity belongs to the scene.
     */
    public boolean hasPoi(Poi poi) {
        return hasPoi(mNativeInstanceAddr, poi.getId());
    }

    /**
     * get the GLSurfaceView.Renderer that uses this Engine as Rendering engine.
     *
     * @return the renderer
     */
    public GLSurfaceView.Renderer getRenderer() {
        return mRenderer;
    }

    /* ***
     * SETTERS
     */

    /**
     * Set the viewport size. Typically used when viewport size changes.
     *
     * @param width  required width of the viewport.
     * @param height required height of the viewport.
     */
    public void setSurfaceSize(final int width, final int height) {
        if (width < 1 || height < 1) {
            throw new IllegalArgumentException("invalid surface size.");
        }
        setSurfaceSize(mNativeInstanceAddr, width, height);
    }

    /**
     * Sets the {@link EngineListener} to be notified on engine event
     * thrown by native implementation.
     *
     * @param callbacks the engine callback
     */
    public void setNativeEngineListener(EngineListener callbacks) {
        // retain a reference to prevent GC to wipe the native callback object.
        mNativeListener.setEngineListener(callbacks);

    }

    @Override
    public void setCameraRotation(float[] rotationMatrix) {
        setCameraRotation(mNativeInstanceAddr, rotationMatrix);
    }

    @Override
    public void setCameraPosition(double lat, double lng, double alt, boolean animated) {
        mCameraPosition[0] = lat;
        mCameraPosition[1] = lng;
        mCameraPosition[2] = alt;
        setCameraPosition(mNativeInstanceAddr, lat, lng, alt, animated);
    }

    @Override
    public void setCameraPosition(double lat, double lon, double alt) {
        setCameraPosition(lat, lon, alt, true);
    }

    @Override
    public void setCameraPosition(double lat, double lon) {
        setCameraPosition(lat, lon, mCameraPosition[2]);
    }

    @Override
    public void setOrigin(double latitude, double longitude) {
        setOrigin(mNativeInstanceAddr, latitude, longitude);
    }

    @Override
    public void setSkyBox(String sid) {
        setSkyBox(mNativeInstanceAddr, sid);
    }

    @Override
    public void setSkyBoxEnabled(boolean enabled) {
        setSkyBoxEnabled(mNativeInstanceAddr, enabled);
    }

    public void setTileNamespace(String namespace) {
        setTileNamespace(mNativeInstanceAddr, namespace);
    }

    /**
     * Steps the engine.
     */
    public void step() {
        step(mNativeInstanceAddr);
    }

    public void updateTileDiffuseMaps() {
        updateTileDiffuseMaps(mNativeInstanceAddr);
    }

    @Override
    public boolean isInstalled() {
        if (mInstalled) {
            return true;
        }

        mInstalled = ArpiGlInstaller.getInstance(mContext).isInstalled();

        return mInstalled;
    }

    /**
     * Get the absolute path where the engine is installed. (the res folder).
     *
     * @return the engine's installation path.
     */
    public String getInstallationDir() {
        return mInstallationDir;
    }

    /* ***
     * PRIVATE METHODS
     */

    private void throwIfNotInit(String func) {
        throwIfNotInstalled(func);
        if (!isInit()) {
            throw new IllegalStateException("cannot call " + func + " while the engine is not initialized");
        }
    }

    private void throwIfNotInstalled(String func) {
        if (!isInstalled()) {
            throw new IllegalStateException("cannot call " + func + " while the engine is not installed");
        }
    }

    /**
     * Use this rendering engine as a renderer for android's GLSurfaceView.
     */
    private class Renderer implements GLSurfaceView.Renderer {
        private final Engine mEngine;

        public Renderer(Engine engine) {
            mEngine = engine;
        }

        /**
         * On draw frame.
         *
         * @see GLSurfaceView.Renderer#onDrawFrame(GL10)
         */
        @Override
        public final void onDrawFrame(GL10 gl) {
            step();
        }

        /**
         * @see GLSurfaceView.Renderer#onSurfaceChanged(GL10, int, int)
         */
        @Override
        public final void onSurfaceChanged(GL10 gl, int width, int height) {
            mEngine.setSurfaceSize(width, height);
        }

        /**
         * @see GLSurfaceView.Renderer#onSurfaceCreated(GL10, EGLConfig)
         */
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.v(TAG, "onSurfaceCreated...");
            if (!mEngine.isInit()) {
                final boolean success = mEngine.init();
                if (BuildConfig.DEBUG && !success) {
                    throw new AssertionError("engine could not initialize.");
                }
            } else {
                mEngine.refresh();
            }
            Log.v(TAG, "onSurfaceCreated done");
        }
    }

    /**
     * This class will be passed to the C++ engine to request tiles
     */
    private class NativeFallthroughEngineListener implements EngineListener {

        /**
         * address of the native c++ object.
         */
        private long mNativeAddr;

        /**
         * Wrapped listener
         */
        private EngineListener mEngineListener;

        /**
         * Gets the native c++ address.
         *
         * @return the native address
         */
        long getNativeAddr() {
            return mNativeAddr;
        }

        public NativeFallthroughEngineListener() {
            mNativeAddr = newEngineListener();
        }

        @Override
        protected void finalize() throws Throwable {
            super.finalize();
            freeEngineListener(mNativeAddr);
        }

        public void setEngineListener(EngineListener engineListener) {
            this.mEngineListener = engineListener;
        }

        @Override
        public void onTileRequest(final int x, final int y, final int z) {
            // MAY DEADLOCK IF RUN IN THE NATIVE THREAD
            new Thread(new Runnable() {
                @Override
                public void run() {
                    if (mEngineListener != null) {
                        mEngineListener.onTileRequest(x, y, z);
                    }
                }
            }).start();
        }

        /* ***
        * ENGINE LISTENER NATIVE METHODS
        */
        private native long newEngineListener();

        private native void freeEngineListener(long nativeAddr);
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

    private native void addPoi(long nativeInstanceAddr, String sid, String shape, String icon, float r, float g, float b, double lat, double lng, double alt);

    private native void removePoi(long nativeInstanceAddr, String sid);

    private native void setPoiPosition(long nativeInstanceAddr, String sid, double lat, double lng, double alt);

    private native void setPoiColor(long nativeInstanceAddr, String sid, float r, float g, float b);

    private native boolean isInit(long nativeInstanceAddr);

    private native boolean isAbleToDraw(long nativeInstanceAddr);

    private native boolean hasPoi(long nativeInstanceAddr, String sid);

    private native void setSkyBox(long nativeInstanceAddr, String sid);

    private native void setSkyBoxEnabled(long nativeInstanceAddr, boolean enabled);

    private native void setSurfaceSize(long nativeInstanceAddr, int width, int height);

    private native void setEngineListener(long nativeInstanceAddr, long nativeCallbackAddr);

    private native void setCameraRotation(long nativeInstanceAddr, float[] rotationMatrix);

    private native void setCameraPosition2d(long nativeInstanceAddr, double lat, double lon);

    private native void setCameraPosition(long nativeInstanceAddr, double lat, double lon, double alt, boolean animated);

    private native void zoom(long nativeInstanceAddr, float offset);

    private native void setOrigin(long nativeInstanceAddr, double lat, double lon);

    private native void notifyTileAvailable(long nativeInstanceAddr, int x, int y, int z);

    private native void setTileNamespace(long nativeInstanceAddr, String namespace);

    private native void updateTileDiffuseMaps(long nativeInstanceAddr);

    private native void selectPoi(long nativeInstanceAddr, int x, int y);

}
