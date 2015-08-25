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

import android.app.Fragment;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.provider.Settings;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import java.util.HashSet;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.engine.Engine;
import mobi.designmyapp.arpigl.engine.impl.AbstractEngineController;
import mobi.designmyapp.arpigl.engine.impl.ArpiGlEngine;
import mobi.designmyapp.arpigl.provider.LocationProvider;
import mobi.designmyapp.arpigl.provider.impl.FusedLocationProvider;
import mobi.designmyapp.arpigl.R;

/**
 * Uses an {@link ArpiGlView} to display a map with POIs. Creates and manages
 * a DMA {@link Engine} to render the scene.
 *
 * @author Nicolas THIERION
 */
public final class ArpiGlFragment extends Fragment {

    /* ***
     * DEBUG CONSTANTS
     */
    @SuppressWarnings("unused")
    private static final String TAG = ArpiGlFragment.class.getSimpleName();

    /* ***
     * REQUEST CODES
     */
    private static final int REQUEST_ENABLE_GPS = 1;

    private static final boolean DEFAULT_TRACK_LOCATION = false;

    /* ***
     * WIDGETS
     */
    /**
     * current app context.
     */
    private static Context mContext;

    /* ***
     * ATTRIBUTES
     */
    /**
     * dispatch location events.
     */
    private final MLocationDispatcher mLocationDisaptcher = new MLocationDispatcher();
    /**
     * View where GL scene will be drawn.
     */
    private ArpiGlView wView;
    /**
     * ArpiGl engine.
     */
    private Engine mEngine;
    /**
     * The location provider.
     */
    private LocationProvider mLocationProvider;
    private boolean mAskedForGps = false;

    // startup value, keep to false
    private boolean mLocationTracking = false;

    /* ***
     * FRAGMENT Overrides
     */
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        final View rootView = inflater.inflate(R.layout.fragment_arpi_gl, container, false);

        // init attributes
        mContext = getActivity().getApplicationContext();

        // init location provider
        mLocationProvider = FusedLocationProvider.getInstance(mContext);

        // load layout.
        wView = (ArpiGlView) rootView.findViewById(R.id.arpigl_glview);

        // install the engine if not installed
        final ArpiGlInstaller installer = ArpiGlInstaller.getInstance(mContext);
        if (!installer.isInstalled()) {
            throw new IllegalStateException("Engine should be installed first");
        }

        // create the engine
        mEngine = new ArpiGlEngine(mContext);

        // finally, set the engine as the renderer for our view.
        wView.setRenderer(mEngine.getRenderer());
        setUserLocationEnabled(DEFAULT_TRACK_LOCATION);

        return rootView;
    }

    @Override
    public void onPause() {
        super.onPause();
        wView.onPause();
        mLocationProvider.unregisterListener(mLocationDisaptcher);
    }

    @Override
    public void onResume() {
        super.onResume();
        wView.onResume();
        //mEngine.resume();

        if (mLocationTracking) {
            if (mLocationProvider.isAvailable()) {
                if (mLocationProvider.isEnabled()) {
                    mLocationProvider.registerListener(mLocationDisaptcher);
                } else if (!mAskedForGps) {
                    showEnableGpsIntent();
                }
            } else {
                mShowNoLocationError();
            }
        }
    }


    @Override
    public void onHiddenChanged(boolean hidden) {
        super.onHiddenChanged(hidden);
        if (hidden) { //pause the OpenGL thread
            wView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            if (mLocationTracking) {
                mLocationProvider.unregisterListener(mLocationDisaptcher);
            }
        } else { //resume the OpenGL thread
            wView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
            if (mLocationTracking) {
                mLocationProvider.registerListener(mLocationDisaptcher);
            }
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_ENABLE_GPS) {
            if (mLocationProvider.isEnabled()) {
                mLocationProvider.registerListener(mLocationDisaptcher);
            }
        }
    }

    @Override
    public void onDestroy() {
        mEngine.destroy();
        super.onDestroy();
    }

    /* ***
     * SETTERS
     */

    /**
     * Enable or disable Location tracking. When enabled, the view will be
     * updated when device's location changes.
     *
     * @param locationTracking true if should enable location tracking.
     */
    public void setUserLocationEnabled(boolean locationTracking) {
        if (!locationTracking && mLocationTracking) {
            mLocationProvider.unregisterListener(mLocationDisaptcher);
        } else if (locationTracking && !mLocationTracking) {
            mLocationProvider.registerListener(mLocationDisaptcher);
            // Set the last know location as the position for the user camera
            Location lastLocation = mLocationProvider.getLastKnownLocation();
            mEngine.setCameraPosition(lastLocation.getLatitude(), lastLocation.getLongitude());
        }
        mLocationTracking = locationTracking;
    }

    /* ***
     * PROTECTED METHODS
     */

    /**
     * @return the {@link AbstractEngineController} that manages this fragment.
     */
    public Engine getEngine() {
        return mEngine;
    }

    private void mShowNoLocationError() {
        // Use the Builder class for convenient dialog construction
        final AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle(R.string.error_no_gps_available_title)
                .setMessage(R.string.error_no_gps_available_message)
                .setCancelable(false).setNeutralButton(android.R.string.ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int id) {
            }
        });
        builder.create().show();
    }

    void showEnableGpsIntent() {
        mAskedForGps = true;
        Toast.makeText(mContext, R.string.location_turn_on_toast, Toast.LENGTH_LONG).show();
        final Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivityForResult(intent, REQUEST_ENABLE_GPS);
    }

    /* ***
     * PRIVATE CLASSES
     */

    public LocationProvider getLocationProvider() {
        return mLocationProvider;
    }

    public void addLocationListener(LocationListener listener) {
        mLocationDisaptcher.add(listener);
    }

    public void removeLocationListener(LocationListener listener) {
        mLocationDisaptcher.remove(listener);

    }

    private class MLocationDispatcher implements LocationListener {

        private final HashSet<LocationListener> mListeners = new HashSet<LocationListener>();

        public void add(LocationListener listener) {
            if (!mListeners.contains(listener)) {
                mListeners.add(listener);
            }
        }

        public void remove(LocationListener listener) {
            mListeners.remove(listener);
        }

        @Override
        public void onLocationChanged(Location location) {
            synchronized (mListeners) {
                for (LocationListener listener : mListeners) {
                    listener.onLocationChanged(location);
                }
            }
        }

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {
            synchronized (mListeners) {
                for (LocationListener listener : mListeners) {
                    listener.onStatusChanged(provider, status, extras);
                }
            }
        }

        @Override
        public void onProviderEnabled(String provider) {
            synchronized (mListeners) {
                for (LocationListener listener : mListeners) {
                    listener.onProviderEnabled(provider);
                }
            }
        }

        @Override
        public void onProviderDisabled(String provider) {
            synchronized (mListeners) {
                for (LocationListener listener : mListeners) {
                    listener.onProviderDisabled(provider);
                }
            }
        }

    }

}
