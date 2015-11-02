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

package mobi.designmyapp.arpigl.provider.impl;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.opengl.Matrix;
import android.util.Log;
import android.view.Surface;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import mobi.designmyapp.arpigl.listener.OrientationListener;
import mobi.designmyapp.arpigl.provider.OrientationProvider;


/**
 * Orientation provider that use the 'ROTATION_VECTOR' to get its orientation.
 * Helper class that make use of google' s SensorManager. Provides a 4x4
 * rotationMatrix to supply to an openGL renderer. The matrix is column first (
 * as expected by openGL ), and axis are corrected to have Z-front, even when
 * screen rotates.
 *
 * @author Nicolas THIERION
 */
public final class RotationVectorOrientationProvider implements OrientationProvider {

    private static final String TAG = RotationVectorOrientationProvider.class.getSimpleName();

    /* ***
     * PARAMS
     */
    private static final int SENSOR_LISTENING_RATE = SensorManager.SENSOR_DELAY_GAME;

    private static final float[] IDENTITY = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    /**
     * activity
     */
    private final Activity mActivity;
    /**
     * actual sensor manager that give us the sensors.
     */
    private final SensorManager mSensorManager;
    /**
     * the rotation vector sensor.
     */
    private final Sensor mRotationVectorSensor;
    /**
     * list of object to notify.
     */
    private final List<OrientationListener> mListeners;
    /**
     * the current rotation matrix.
     */
    private final float[] mRotationMatrix = Arrays.copyOf(
        IDENTITY,
        IDENTITY.length);
    private final float[] mTmpMatrix = new float[IDENTITY.length];
    /**
     * will listen to rotation vector events.
     */
    private final SensorEventListener mSensorListener = new CustomSensorEventListener();
    /**
     * is some new values has been polled.
     */
    private boolean mHasNewValue = false;

    /* ***
     * CONSTRUCTORS
     */

    private RotationVectorOrientationProvider(Activity activity) {
        mActivity = activity;
        // get sensor manager
        mSensorManager = (SensorManager) mActivity.getSystemService(Context.SENSOR_SERVICE);

        // get rotation vector sensor.
        mRotationVectorSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);

        // init listeners list
        mListeners = new LinkedList<>();
    }

    /**
     * Get the instance of this orientaton provider.
     *
     * @param activity The activity context.
     * @return the RotationVectorOrientationProvider instance.
     */
    public static RotationVectorOrientationProvider getInstance(Activity activity) {
        return new RotationVectorOrientationProvider(activity);
    }

    @Override
    public float[] getRotationMatrix() {
        synchronized (mRotationMatrix) {
            return mRotationMatrix;
        }
    }

    @Override
    public void registerListener(OrientationListener listener) {
        // checks that sensor is available.
        if (!isAvailable()) {
            //            if (BuildConfig.DEBUG) {
            //                throw new AssertionError(
            //                        "This device doesn't have the required sensors. This provider cannot work.");
            //            }
            return;
        }

        synchronized (mListeners) {
            if (!mListeners.contains(listener)) {
                mListeners.add(listener);
            }
            // if listeners just got filled, wake up listening.
            if (mListeners.size() == 1) {
                Log.v(TAG, "waking up listening to ROTATION_VECTOR");
                mSensorManager.registerListener(mSensorListener, mRotationVectorSensor,
                    SENSOR_LISTENING_RATE);
            }
        }
    }

    @Override
    public void unregisterListener(OrientationListener listener) {
        synchronized (mListeners) {
            mListeners.remove(listener);
            // if listeners is empty, wake up listening.
            if (mListeners.isEmpty()) {
                Log.v(TAG, "stop listening to ROTATION_VECTOR");
                mSensorManager.unregisterListener(mSensorListener);
            }
        }
    }

    /* ***
     * PRIVATE FUNCTIONS
     */
    private void mNotifyListeners(SensorEvent event) {
        synchronized (mListeners) {
            for (final OrientationListener listener : mListeners) {
                listener.onOrientationChanged(event);
            }
        }
    }

    @Override
    public boolean isAvailable() {
        if (mRotationVectorSensor == null) {
            Log.w(TAG, "This device doesn't have the required sensors. This provider cannot work.");
        }
        return mRotationVectorSensor != null;
    }

    @Override
    public boolean hasNewValue() {
        final boolean res = mHasNewValue;
        mHasNewValue = false;
        return res;
    }

    /* ***
     * PRIVATE LISTENER
     */
    private class CustomSensorEventListener implements SensorEventListener {

        @Override
        public void onSensorChanged(final SensorEvent event) {

            if (event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR) {
                synchronized (mRotationMatrix) {
                    SensorManager.getRotationMatrixFromVector(mRotationMatrix, event.values);

                    final int mScreenRotation = mActivity.getWindowManager().getDefaultDisplay()
                        .getRotation();

                    switch (mScreenRotation) {
                        case Surface.ROTATION_90:
                            // x => y   &&    y => -x
                            SensorManager.remapCoordinateSystem(mRotationMatrix, SensorManager.AXIS_Y, SensorManager.AXIS_MINUS_X, mTmpMatrix);
                            System.arraycopy(mTmpMatrix, 0, mRotationMatrix, 0, mTmpMatrix.length);
                            break;
                        case Surface.ROTATION_270:
                            // x => -y   &&    y => x
                            SensorManager.remapCoordinateSystem(mRotationMatrix, SensorManager.AXIS_MINUS_Y, SensorManager.AXIS_X, mTmpMatrix);
                            System.arraycopy(mTmpMatrix, 0, mRotationMatrix, 0, mTmpMatrix.length);
                            break;
                        default:
                            break;
                    }

                    // Rotation of 90 degrees about x axis
                    Matrix.rotateM(mRotationMatrix, 0, 90, 1, 0, 0);

                    // we just got a new value. Update 'new' flag.
                    mHasNewValue = true;
                }

                // finally, throw the event to listeners.
                mNotifyListeners(event);
            }

        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
            // NO OP
        }
    }

}
