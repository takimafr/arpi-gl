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

package mobi.designmyapp.arpigl.ui;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import mobi.designmyapp.arpigl.ArpiGlInstaller;

/**
 * Opens an openGL context & init the proper rendering engine display a 3D 'poi
 * scene'. Must be used only through an {@link ArpiGlFragment}.
 *
 * @author Nicolas THIERION.
 */
public class ArpiGlView extends GLSurfaceView {

    /**
     * debug flag.
     */
    private static final String TAG = ArpiGlView.class.getSimpleName();

    private static final boolean DEBUG = false;

    /* ***
     * VIEW CONSTANTS
     */
    private static final boolean DEFAULT_TRANSLUCENT = false;
    /**
     * RGBA_5650.
     */
    private static final byte[] DEFAULT_RGBA_DEPTH = {5, 6, 5, 0};
    private static final byte DEPTH_SIZE_BITS = 16;  //0 will disable the feature
    private static final byte STENCIL_SIZE = 8;   //0 will disable the feature
    /* ***
     * ATTRIBUTES
     */
    /**
     * View surface should be translucent?
     */
    private final boolean mTranslucent = DEFAULT_TRANSLUCENT;

    private final Context mContext;

    /* ***
     * CONSTRUCTORS
     */

    /**
     * Constructor intended to be called by layout inflater.
     *
     * @param context the application context
     */
    public ArpiGlView(Context context) {
        super(context);
        mContext = context;
        mInit();

    }

    /**
     * Constructor intended to be called by layout inflater.
     *
     * @param context the application context
     * @param attrs   the application attributes
     */
    public ArpiGlView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;
        mInit();

    }

    private static boolean mHasEglError(String prompt, EGL10 egl) {
        boolean hasError = false;
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
            hasError = true;
            Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
        }
        return hasError;
    }

    @Override
    public final void onResume() {
        super.onResume();
    }

    @Override
    public final void onPause() {
        super.onPause();
    }

    private void mInit() {
        mInitGlContext();
    }


    /* ***
     * PRIVATE FUNCTIONS
     */

    /**
     * Init openGL ES context. Set view pixelFormat (OPAQUE / TRANSLUCENT...) &
     * colors / z / stencil depth.
     */
    private void mInitGlContext() {

        /*
         * By default, GLSurfaceView() creates a RGB_565 opaque surface. If we
         * want a translucent one, we should change the surface's format here,
         * using PixelFormat.TRANSLUCENT for GL Surfaces is interpreted as any
         * 32-bit surface with alpha by SurfaceFlinger.
         *
         * We need to choose an EGLConfig that matches the format of our surface
         * exactly. This is going to be done in our custom config chooser.
         */
        int format = PixelFormat.OPAQUE;
        byte[] rgbaDepth = DEFAULT_RGBA_DEPTH;

        if (mTranslucent) {
            format = PixelFormat.TRANSLUCENT;
            rgbaDepth = new byte[]{Byte.SIZE, Byte.SIZE, Byte.SIZE, Byte.SIZE};
        }
        final ConfigChooser configChooser = new ConfigChooser(rgbaDepth, DEPTH_SIZE_BITS, STENCIL_SIZE);

        this.getHolder().setFormat(format);
        setEGLConfigChooser(configChooser);

        /*
         * Setup the context factory for 2.0 rendering. See ContextFactory class
         * definition below
         */
        setEGLContextFactory(new ContextFactory());
        if (!ArpiGlInstaller.getInstance(mContext).isInstalled()) {
            Log.i(TAG, "provided engine has not been installed yet.");
        }
        setDebugFlags(DEBUG_LOG_GL_CALLS | DEBUG_CHECK_GL_ERROR);
    }

    /* ***
     * PRIVATE CLASSES
     */
    private static class ConfigChooser implements EGLConfigChooser {

        /*
         * This EGL config specification is used to specify 2.0 rendering. We
         * use a minimum size of 4 bits for red/green/blue, but will perform
         * actual matching in chooseConfig() below.
         */
        private static final int EGL_OPENGL_ES2_BIT = 4;
        private static final int[] DEFAULT_CONFIG = {
                EGL10.EGL_RED_SIZE,
                4,
                EGL10.EGL_GREEN_SIZE,
                4,
                EGL10.EGL_BLUE_SIZE,
                4,
                EGL10.EGL_RENDERABLE_TYPE,
                EGL_OPENGL_ES2_BIT,
                EGL10.EGL_NONE};
        private final int mRedSize;
        private final int mGreenSize;
        private final int mBlueSize;
        private final int mAlphaSize;
        private final int mDepthSize;
        private final int mStencilSize;
        private final int[] mValue = new int[1];
        public ConfigChooser(byte[] rgbaDepth, int depth, int stencil) {
            int i = 0;
            mRedSize = rgbaDepth[i++];
            mGreenSize = rgbaDepth[i++];
            mBlueSize = rgbaDepth[i++];
            mAlphaSize = rgbaDepth[i++];
            mDepthSize = depth;
            mStencilSize = stencil;
        }

        @Override
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

            /*
             * Get the number of minimally matching EGL configurations
             */
            final int[] pNumConfigs = new int[1];
            egl.eglChooseConfig(display, DEFAULT_CONFIG, null, 0, pNumConfigs);

            final int numConfigs = pNumConfigs[0];

            if (numConfigs <= 0) {
                throw new IllegalArgumentException(
                        "No configs match configSpec");
            }

            /*
             * Allocate then read the array of minimally matching EGL configs
             */
            final EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, DEFAULT_CONFIG, configs, numConfigs, pNumConfigs);

            if (DEBUG) {
                printConfigs(egl, display, configs);
            }
            /*
             * Now return the "best" one
             */
            return chooseConfig(egl, display, configs);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) {
            for (final EGLConfig config : configs) {
                final int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, DEPTH_SIZE_BITS);
                final int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, STENCIL_SIZE);

                // We need at least mDepthSize and mStencilSize bits
                if (d < mDepthSize || s < mStencilSize) {
                    continue;
                }

                // We want an *exact* match for red/green/blue/alpha
                final int r = findConfigAttrib(egl, display, config,
                        EGL10.EGL_RED_SIZE, 0);
                final int g = findConfigAttrib(egl, display, config,
                        EGL10.EGL_GREEN_SIZE, 0);
                final int b = findConfigAttrib(egl, display, config,
                        EGL10.EGL_BLUE_SIZE, 0);
                final int a = findConfigAttrib(egl, display, config,
                        EGL10.EGL_ALPHA_SIZE, 0);

                if (r == mRedSize && g == mGreenSize && b == mBlueSize
                        && a == mAlphaSize) {
                    return config;
                }
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                                     EGLConfig config, int attribute, int defaultValue) {

            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                return mValue[0];
            }
            return defaultValue;
        }

        private void printConfigs(EGL10 egl, EGLDisplay display,
                                  EGLConfig[] configs) {
            final int numConfigs = configs.length;
            Log.i(TAG, String.format("%d configurations", numConfigs));
            for (int i = 0; i < numConfigs; i++) {
                Log.i(TAG, String.format("Configuration %d:\n", i));
                printConfig(egl, display, configs[i]);
            }
        }

        private void printConfig(EGL10 egl, EGLDisplay display, EGLConfig config) {
            final int[] attributes = {EGL10.EGL_BUFFER_SIZE,
                    EGL10.EGL_ALPHA_SIZE, EGL10.EGL_BLUE_SIZE,
                    EGL10.EGL_GREEN_SIZE,
                    EGL10.EGL_RED_SIZE,
                    EGL10.EGL_DEPTH_SIZE,
                    EGL10.EGL_STENCIL_SIZE,
                    EGL10.EGL_CONFIG_CAVEAT,
                    EGL10.EGL_CONFIG_ID,
                    EGL10.EGL_LEVEL,
                    EGL10.EGL_MAX_PBUFFER_HEIGHT,
                    EGL10.EGL_MAX_PBUFFER_PIXELS,
                    EGL10.EGL_MAX_PBUFFER_WIDTH,
                    EGL10.EGL_NATIVE_RENDERABLE,
                    EGL10.EGL_NATIVE_VISUAL_ID,
                    EGL10.EGL_NATIVE_VISUAL_TYPE,
                    0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                    EGL10.EGL_SAMPLES,
                    EGL10.EGL_SAMPLE_BUFFERS,
                    EGL10.EGL_SURFACE_TYPE,
                    EGL10.EGL_TRANSPARENT_TYPE,
                    EGL10.EGL_TRANSPARENT_RED_VALUE,
                    EGL10.EGL_TRANSPARENT_GREEN_VALUE,
                    EGL10.EGL_TRANSPARENT_BLUE_VALUE,
                    0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
                    0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
                    0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
                    0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
                    EGL10.EGL_LUMINANCE_SIZE, EGL10.EGL_ALPHA_MASK_SIZE,
                    EGL10.EGL_COLOR_BUFFER_TYPE, EGL10.EGL_RENDERABLE_TYPE,
                    0x3042 // EGL10.EGL_CONFORMANT
            };
            final String[] names = {"EGL_BUFFER_SIZE", "EGL_ALPHA_SIZE",
                    "EGL_BLUE_SIZE", "EGL_GREEN_SIZE", "EGL_RED_SIZE",
                    "EGL_DEPTH_SIZE", "EGL_STENCIL_SIZE", "EGL_CONFIG_CAVEAT",
                    "EGL_CONFIG_ID", "EGL_LEVEL", "EGL_MAX_PBUFFER_HEIGHT",
                    "EGL_MAX_PBUFFER_PIXELS", "EGL_MAX_PBUFFER_WIDTH",
                    "EGL_NATIVE_RENDERABLE", "EGL_NATIVE_VISUAL_ID",
                    "EGL_NATIVE_VISUAL_TYPE", "EGL_PRESERVED_RESOURCES",
                    "EGL_SAMPLES", "EGL_SAMPLE_BUFFERS", "EGL_SURFACE_TYPE",
                    "EGL_TRANSPARENT_TYPE", "EGL_TRANSPARENT_RED_VALUE",
                    "EGL_TRANSPARENT_GREEN_VALUE",
                    "EGL_TRANSPARENT_BLUE_VALUE", "EGL_BIND_TO_TEXTURE_RGB",
                    "EGL_BIND_TO_TEXTURE_RGBA", "EGL_MIN_SWAP_INTERVAL",
                    "EGL_MAX_SWAP_INTERVAL", "EGL_LUMINANCE_SIZE",
                    "EGL_ALPHA_MASK_SIZE", "EGL_COLOR_BUFFER_TYPE",
                    "EGL_RENDERABLE_TYPE", "EGL_CONFORMANT"};
            final int[] value = new int[1];
            for (int i = 0; i < attributes.length; i++) {
                final int attribute = attributes[i];
                final String name = names[i];
                if (egl.eglGetConfigAttrib(display, config, attribute, value)) {
                    Log.i(TAG, String.format("  %s: %d\n", name, value[0]));
                } else {
                    Log.e(TAG, String.format("  %s: failed\n", name));
                    int glError;
                    do {
                        glError = egl.eglGetError();
                    } while (glError != EGL10.EGL_SUCCESS);
                }
            }
        }
    }

    private static class ContextFactory implements EGLContextFactory {
        private static final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

        @Override
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            Log.i(TAG, "creating OpenGL ES 2.0 context");
            mHasEglError("Before eglCreateContext", egl);
            final int[] attribList = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE};
            final EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attribList);
            mHasEglError("After eglCreateContext", egl);
            Log.i(TAG, "OpenGL ES 2.0 context created");
            return context;
        }

        @Override
        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }
}
