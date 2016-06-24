package mobi.designmyapp.arpigl.engine;

import android.content.Context;
import android.os.Handler;

import mobi.designmyapp.arpigl.listener.EngineListener;

/**
 * This class will be passed to the C++ engine to request tiles
 */
public class NativeFallthroughEngineListener implements EngineListener {

    /**
     * address of the native c++ object.
     */
    private long mNativeAddr;

    /**
     * android context
     */
    private Context mContext;
    /**
     * Wrapped listener
     */
    private EngineListener mEngineListener;

    public NativeFallthroughEngineListener(Context mContext) {
        this.mContext = mContext;
        mNativeAddr = newEngineListener();
    }
    /**
     * Gets the native c++ address.
     *
     * @return the native address
     */
    long getNativeAddr() {
        return mNativeAddr;
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
        final Handler mainHandler = new Handler(mContext.getMainLooper());
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (mEngineListener != null) {
                    mEngineListener.onTileRequest(x, y, z);
                }
            }
        };
        mainHandler.post(runnable);
    }

    @Override
    public void onPoiSelected(final String sid) {
        // MAY DEADLOCK IF RUN IN THE NATIVE THREAD
        final Handler mainHandler = new Handler(mContext.getMainLooper());
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (mEngineListener != null) {
                    mEngineListener.onPoiSelected(sid);
                }
            }
        };
        mainHandler.post(runnable);
    }

    @Override
    public void onPoiDeselected(final String sid) {
        // MAY DEADLOCK IF RUN IN THE NATIVE THREAD
        final Handler mainHandler = new Handler(mContext.getMainLooper());
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (mEngineListener != null) {
                    mEngineListener.onPoiDeselected(sid);
                }
            }
        };
        mainHandler.post(runnable);
    }

    /* ***
    * ENGINE LISTENER NATIVE METHODS
    */
    private native long newEngineListener();

    private native void freeEngineListener(long nativeAddr);
}