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

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.BuildConfig;
import mobi.designmyapp.arpigl.engine.Engine;
import mobi.designmyapp.arpigl.listener.EngineListener;
import mobi.designmyapp.arpigl.model.Poi;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;

/**
 * Draws an OpenGL scene to display some POIs on a map. Wraps native cpp class
 * 'dma::Engine'.
 *
 * @author Nicolas THIERION
 * @version 0.2.3
 */
public final class ArpiGlEngine implements Engine {

    /**
     * resource folder stored in assets, that must contain engine installation
     * files. Any file in this folder will be copied into the device's internal
     * storage.
     */
    public static final String RESOURCES_FOLDER = ArpiGlInstaller.INSTALLATION_DIR;
    /**
     * Debug tag.
     */
    private static final String TAG = ArpiGlEngine.class.getSimpleName();
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
     * path of Engine's root dir where files are copied.
     */
    private final String mFilesDir;
    /**
     * context of the application.
     */
    private final Context mContext;
    /**
     * current camera.
     */
    private final Camera mCurrentCamera = new Camera();
    /**
     * Renderer for android's GLSurfaceView that uses this Engine as Rendering
     * engine.
     */
    private final Renderer mRenderer;
    /**
     * Bridge that will route each engine call to the native Engine object.
     */
    private final ArpiGlEngineBridge mBridge;
    /**
     * flag to tell if engine is installed.
     */
    private boolean mInstalled;
    /**
     * callbacks to be notified on native engine event. This callback is set by
     * default to discard events.
     */
    private EngineListener mCallbacks = new NullListener();

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
    public ArpiGlEngine(Context context) {

        mContext = context;

        // set the installation path, where resource assets are (will be)
        // copied.
        final StringBuilder filesDirBuilder = new StringBuilder();
        filesDirBuilder.append(context.getFilesDir().getAbsolutePath());
        if (filesDirBuilder.charAt(filesDirBuilder.length() - 1) != '/') {
            filesDirBuilder.append('/');
        }
        filesDirBuilder.append(RESOURCES_FOLDER);
        mFilesDir = filesDirBuilder.toString();

        // Connect this Engine to native implementation through a bridge.
        // As we want the bridge to be effective only from
        // the point engine is initialized,
        // use an EngineManagedBridge that will properly connect or disconnect
        // the bridge during engine's lifecycle.
        mBridge = new ArpiGlEngineBridge(this);
        mRenderer = new Renderer(this);
        // register native callbacks
        mBridge.setNativeCallbacks(new FallthroughListener());
        setCameraPosition(0.0, 0.0, 5.0);
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
     * Destroy this engine. The engine should then not be used anymore.
     */
    @Override
    public void destroy() {
        mBridge.destroy();
    }

    @Override
    public boolean init() {
        throwIfNotInstalled("init()");
        return mBridge.init();
    }

    @Override
    public void pause() {
        if (isInstalled()) {
            if (isInit()) {
                mBridge.wipe();
            }
        }
    }

    @Override
    public void resume() {
        if (isInstalled()) {
            // as we probably lost openGL context, we need to re-upload
            // resources from CPU memory to GPU memory.
            if (isInit()) {
                refresh();
            }
        }
    }

    /* ***
     * API METHODS
     */

    /**
     * Remember to call this method before the Application context is unloaded.
     * Android will trash the openGL on context change, like for example when
     * the application is paused. The internal context will thus have to be
     * refreshed. After this call, the engine context should be re-loaded
     * through {@link #refresh}.
     */
    @Override
    public void wipe() {
        if (isInit()) {
            Log.v(TAG, "Wiping engine");
            mBridge.wipe();
            Log.v(TAG, "Engine wiped");
        } else {
            Log.w(TAG, "wipe but bridge is not init");
            if (BuildConfig.DEBUG) {
                throwIfNotInit("wipe()");
            }
        }
    }

    @Override
    public void refresh() {
        throwIfNotInstalled("refresh()");
        if (isInit()) {
            mBridge.refresh();
        } else {
            Log.w(TAG, "refresh but bridge is not init");
            if (BuildConfig.DEBUG) {
                throwIfNotInit("refresh()");
            }
        }
    }

    @Override
    public synchronized void unload() {
        mBridge.unload();
    }

    @Override
    public void addPoi(Poi poi) {
        mBridge.addPoi(poi);
    }

    @Override
    public void removePoi(Poi poi) throws IllegalStateException {
        mBridge.removePoi(poi);
    }

    @Override
    public void setPoiPosition(String sid, double lat, double lng, double alt) {
        mBridge.setPoiPosition(sid, lat, lng, alt);
    }

    @Override
    public void setPoiColor(String sid, int color) {
        mBridge.setPoiColor(sid, color);
    }

    @Override
    public void notifyTileAvailable(final int x, final int y, final int z) {
        mBridge.notifyTileAvailable(x, y, z);
    }

    /* ***
     * GETTERS
     */

    @Override
    public boolean isInit() {
        return mBridge.isInit();
    }

    @Override
    public boolean isAbleToDraw() {
        return mBridge.isAbleToDraw();
    }

    @Override
    public boolean hasPoi(Poi poi) {
        return mBridge.hasPoi(poi);
    }

    /**
     * get the GLSurfaceView.Renderer that uses this Engine as Rendering engine.
     *
     * @return the renderer
     */
    @Override
    public GLSurfaceView.Renderer getRenderer() {
        return mRenderer;
    }

    @Override
    public float[] getCameraRotationMatrix() {
        synchronized (mCurrentCamera) {
            final float[] cachedMatrix = mCurrentCamera.cachedRotationMatrix;
            final float[] matrix = mCurrentCamera.rotationMatrix;
            System.arraycopy(matrix, 0, cachedMatrix, 0, matrix.length);
            return cachedMatrix;
        }
    }

    /* ***
     * SETTERS
     */

    @Override
    public void setSurfaceSize(final int width, final int height) {
        if (width < 1 || height < 1) {
            throw new IllegalArgumentException("invalid surface size.");
        }
        mBridge.setSurfaceSize(width, height);

    }

    @Override
    public void setNativeCallbacks(EngineListener callbacks) {
        // retain a reference to prevent GC to wipe the native callback object.
        mCallbacks = callbacks;
    }

    @Override
    public void setCameraRotation(float[] rotationMatrix) {
        synchronized (mCurrentCamera) {
            System.arraycopy(rotationMatrix, 0, mCurrentCamera.rotationMatrix, 0, rotationMatrix.length);
            mBridge.setCameraRotation(rotationMatrix);
        }
    }

    @Override
    public void setCameraPosition(double lat, double lon, double alt) {
        mBridge.setCameraPosition(lat, lon, alt);
    }

    @Override
    public void setCameraPosition(double lat, double lon) {
        mBridge.setCameraPosition(lat, lon);
    }

    @Override
    public void setOrigin(double latitude, double longitude) {
        mBridge.setOrigin(latitude, longitude);
    }

    @Override
    public void setSkyBox(String sid) {
        mBridge.setSkyBox(sid);
    }

    @Override
    public void setSkyBoxEnabled(boolean enabled) {
        mBridge.setSkyBoxEnabled(enabled);
    }

    @Override
    public void step() {
        mBridge.step();
    }

    @Override
    public boolean isInstalled() {
        if (mInstalled) {
            return true;
        }

        mInstalled = ArpiGlInstaller.getInstance(mContext).isInstalled();

        return mInstalled;
    }

    @Override
    public String getInstallationDir() {
        return mFilesDir;
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

    private static class Camera {

        /**
         * current rotation matrix.
         */
        private final float[] rotationMatrix = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

        private final float[] cachedRotationMatrix = new float[rotationMatrix.length];
    }

    /**
     * Use this rendering engine as a renderer for android's GLSurfaceView.
     */
    private class Renderer implements GLSurfaceView.Renderer {
        private final ArpiGlEngine mEngine;

        public Renderer(ArpiGlEngine engine) {
            mEngine = engine;
        }

        /**
         * On draw frame.
         *
         * @see GLSurfaceView.Renderer#onDrawFrame(GL10)
         */
        @Override
        public final void onDrawFrame(GL10 gl) {
            mBridge.setCameraRotation(mCurrentCamera.rotationMatrix);
            mBridge.step();
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
                mEngine.refresh(); // FIXME refresh twice due to an unknown bug
            }
            Log.v(TAG, "onSurfaceCreated done");
        }
    }

    private class FallthroughListener extends AbstractEngineListener {

        @Override
        public void onTileRequest(final int x, final int y, final int z) {
            // MAY DEADLOCK IF RUN IN THE NATIVE THREAD
            new Thread(new Runnable() {
                @Override
                public void run() {
                    mCallbacks.onTileRequest(x, y, z);
                }
            }).start();
        }
    }

    private class NullListener extends AbstractEngineListener {
        @Override
        public void onTileRequest(final int x, final int y, final int z) {
            Log.d(TAG, "ignored onTileRequest tile (" + x + ", " + y + ", " + z + ")");
        }
    }

}
