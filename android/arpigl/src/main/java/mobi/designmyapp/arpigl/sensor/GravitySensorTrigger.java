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

package mobi.designmyapp.arpigl.sensor;

import android.app.Activity;
import android.app.Application;
import android.app.Fragment;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;

import java.lang.ref.WeakReference;

/**
 * This class listen to GRAVITY_SENSOR, and show or hide the provided fragment
 * according to device's gravity.
 */
public class GravitySensorTrigger extends VisibilityTrigger {

    /**
     * The fragment will be shown when sensor's value is above this threshold.
     * by default.
     */
    public static final float DEFAULT_SHOW_THRESHOLD = 20.0f;
    /**
     * The fragment will be hidden when sensor's value go bellow this threshold
     * by default.
     */
    public static final float DEFAULT_HIDE_THRESHOLD = 90.0f;
    /* ***
     * CONSTANTS
     */
    private static final String TAG = GravitySensorTrigger.class.getSimpleName();
    private static final float GRAVITY_CONSTANT = 9.81f;

    /* ***
     * ATTRIBUTES
     */
    /**
     * sensor used to determine if we should toggle visibility.
     */
    private final Sensor mGravitySensor;
    /**
     * Keep a reference to the activity, without leaking the context.
     */
    private final WeakReference<Activity> mActivityContext;
    /**
     * Android sensor manager to get access to sensors.
     */
    private SensorManager mSensorManager;
    private ActivityCallbacks mActivityLifecycleCallbacks;

    private boolean mEnabled = false;

    private float mShowTreshold = DEFAULT_SHOW_THRESHOLD;

    private float mHideTreshold = DEFAULT_HIDE_THRESHOLD;

    private SensorEventListener mSensorEventListener = new InnerSensorEventListener();
    private boolean mDestroyed = false;

    /* ***
     * CONSTRUCTOR
     */

    /**
     * Create a new {@code GravitySensorTrigger} to manage the visibility of
     * given fragment. The created trigger is enabled right after the
     * construction is done.
     *
     * @param fragment the fragment to handle visibility.
     */
    public GravitySensorTrigger(Fragment fragment) {
        super(fragment);
        mActivityContext = new WeakReference<>(fragment.getActivity());
        mActivityLifecycleCallbacks = new ActivityCallbacks();

        mSensorManager = (SensorManager) mActivityContext.get().getSystemService(Context.SENSOR_SERVICE);
        mGravitySensor = mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
        setEnabled(true);
    }

    @Override
    public final boolean isEnabled() {
        return mEnabled;
    }

    @Override
    public final void setEnabled(boolean enabled) {
        if (!mEnabled && enabled) {
            if (mGravitySensor == null) {
                Log.e(TAG, "No Gravity Sensor");
            } else {
                mSensorManager
                        .registerListener(mSensorEventListener, mGravitySensor, SensorManager.SENSOR_DELAY_NORMAL);
            }
            mActivityContext.get().getApplication().registerActivityLifecycleCallbacks(mActivityLifecycleCallbacks);

        } else if (mEnabled && !enabled) {
            mSensorManager.unregisterListener(mSensorEventListener);
        }
        mEnabled = enabled;
    }

    /**
     * Set the show-threshold value. Fragment will be shown when sensor's value
     * will be above this threshold.
     *
     * @param showTreshold - threshold value.
     */
    public final void setShowTreshold(float showTreshold) {
        mShowTreshold = showTreshold;
        // trick to ensure showTreshold is lower than hideTreshold
        if (mShowTreshold > mHideTreshold) {
            mHideTreshold = mShowTreshold + 1;
        }
    }

    /**
     * Set the hide-threshold value. Fragment will be hidden when sensor's value
     * will be below this threshold.
     *
     * @param hideTreshold - threshold value.
     */
    public final void setHideTreshold(float hideTreshold) {
        mHideTreshold = hideTreshold;
        // trick to ensure showTreshold is lower than hideTreshold
        if (mShowTreshold > mHideTreshold) {
            mShowTreshold = mHideTreshold - 1;
        }
    }

    /* ***
     * PRIVATE CLASSES
     */

    private class InnerSensorEventListener implements SensorEventListener {
        @Override
        public void onSensorChanged(SensorEvent event) {
            if (event.sensor.getType() == Sensor.TYPE_GRAVITY) {

                final float percent = (event.values[2] / GRAVITY_CONSTANT) * 100.0f;
                if (!isShown() && percent < mShowTreshold) {
                    synchronized (mActivityContext) {
                        if (!isShown() && percent < mShowTreshold && !mDestroyed) {
                            show(true);
                        }
                    }
                } else if (isShown() && percent > mHideTreshold) {
                    synchronized (mActivityContext) {
                        if (isShown() && percent > mHideTreshold && !mDestroyed) {
                            show(false);
                        }
                    }
                }
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {

        }
    }

    private class ActivityCallbacks implements Application.ActivityLifecycleCallbacks {

        @Override
        public void onActivityCreated(Activity activity, Bundle savedInstanceState) {

        }

        @Override
        public void onActivityStarted(Activity activity) {

        }

        @Override
        public void onActivityResumed(Activity activity) {
            if (!activity.equals(mActivityContext.get())) {
                return;
            }
            synchronized (mActivityContext) {
                if (mGravitySensor == null) {
                    Log.e(TAG, "No Gravity Sensor");
                    return;
                }
                if (mEnabled) {
                    mSensorManager.registerListener(mSensorEventListener, mGravitySensor,
                            SensorManager.SENSOR_DELAY_NORMAL);
                }
            }
        }

        @Override
        public void onActivityPaused(Activity activity) {
            if (!activity.equals(mActivityContext.get())) {
                return;
            }
            synchronized (mActivityContext) {
                if (mEnabled) {
                    mSensorManager.unregisterListener(mSensorEventListener);
                }
            }
        }

        @Override
        public void onActivityStopped(Activity activity) {

        }

        @Override
        public void onActivitySaveInstanceState(Activity activity, Bundle outState) {

        }

        @Override
        public void onActivityDestroyed(Activity activity) {
            if (!activity.equals(mActivityContext.get())) {
                return;
            }

            synchronized (mActivityContext) {
                mDestroyed = true;

                Log.v(TAG, "onActivityDestroy: unregister callbacks from activity");
                mActivityContext.get().getApplication()
                        .unregisterActivityLifecycleCallbacks(mActivityLifecycleCallbacks);

            }
        }
    }

}
