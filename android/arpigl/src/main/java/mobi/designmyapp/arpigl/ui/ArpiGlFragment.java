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
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.provider.Settings;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.R;
import mobi.designmyapp.arpigl.engine.Engine;
import mobi.designmyapp.arpigl.provider.LocationProvider;
import mobi.designmyapp.arpigl.provider.impl.FusedLocationProvider;

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
     * View where GL scene will be drawn.
     */
    private ArpiGlView mArpiGlView;
    /**
     * ArpiGl engine.
     */
    private Engine mEngine;
    /**
     * The location provider.
     */
    private LocationProvider mLocationProvider;

    private boolean mOpenLocationSettings = true;

    // startup value, keep to false
    private boolean mLocationTracking = false;

    private NetworkStateBroadcastReceiver mNetworkStateBroadcastReceiver;
    private LocationStateBroadcastReceiver mLocationStateBroadcastReceiver;

    /* ***
     * FRAGMENT Overrides
     */
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        final View rootView = inflater.inflate(R.layout.fragment_arpi_gl, container, false);

        // init attributes
        mContext = getActivity().getApplicationContext();

        // load layout.
        mArpiGlView = (ArpiGlView) rootView.findViewById(R.id.arpigl_glview);

        // install the engine if not installed
        final ArpiGlInstaller installer = ArpiGlInstaller.getInstance(mContext);
        if (!installer.isInstalled()) {
            throw new IllegalStateException("Engine should be installed first");
        }

        // create the engine
        mEngine = new Engine(mContext);

        mLocationProvider = FusedLocationProvider.getInstance(mContext);
        // finally, set the engine as the renderer for our view.
        mArpiGlView.setRenderer(mEngine.getRenderer());
        setUserLocationEnabled(DEFAULT_TRACK_LOCATION);

        mNetworkStateBroadcastReceiver = new NetworkStateBroadcastReceiver();
        mLocationStateBroadcastReceiver = new LocationStateBroadcastReceiver();

        return rootView;
    }

    @Override
    public void onPause() {
        super.onPause();
        mArpiGlView.onPause();
        mLocationProvider.setListeningEnabled(false);
        mContext.unregisterReceiver(mNetworkStateBroadcastReceiver);
        mContext.unregisterReceiver(mLocationStateBroadcastReceiver);
    }

    @Override
    public void onResume() {
        super.onResume();
        mArpiGlView.onResume();

        if (mLocationTracking) {
            if (mLocationProvider.isAvailable()) {
                if (!mLocationProvider.isListening()) {
                    if (mOpenLocationSettings) {
                        showEnableGpsIntent();
                    } else {
                        Toast.makeText(mContext, R.string.location_turn_on_toast, Toast.LENGTH_LONG).show();
                    }
                } else {
                    mLocationProvider.setListeningEnabled(true);
                }
            } else {
                mShowNoLocationError();
            }
        }

        // register network state broadcast receiver
        IntentFilter networkStateIntentFilter = new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION);
        mContext.registerReceiver(mNetworkStateBroadcastReceiver, networkStateIntentFilter);

        // register location state broadcast receiver
        IntentFilter locationStateIntentFilter = new IntentFilter(LocationManager.PROVIDERS_CHANGED_ACTION);
        mContext.registerReceiver(mLocationStateBroadcastReceiver, locationStateIntentFilter);
    }


    @Override
    public void onHiddenChanged(boolean hidden) {
        super.onHiddenChanged(hidden);
        if (hidden) { //pause the OpenGL thread
            mArpiGlView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            if (mLocationTracking) {
                mLocationProvider.setListeningEnabled(false);
            }
        } else { //resume the OpenGL thread
            mArpiGlView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
            if (mLocationTracking) {
                mLocationProvider.setListeningEnabled(true);
            }
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_ENABLE_GPS && mLocationTracking) {
            mLocationProvider.setListeningEnabled(true);
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
            mLocationProvider.setListeningEnabled(false);
        } else if (locationTracking && !mLocationTracking) {
            mLocationProvider.setListeningEnabled(true);
            // Set the last know location as the position for the user camera
            Location lastLocation = mLocationProvider.getLastKnownLocation();

            if (lastLocation == null) {
                lastLocation = new Location(LocationManager.PASSIVE_PROVIDER);
                lastLocation.setLatitude(0);
                lastLocation.setLongitude(0);
            }
            mEngine.setCameraPosition(lastLocation.getLatitude(), lastLocation.getLongitude());
        }
        mLocationTracking = locationTracking;
    }

    public void setOpenLocationSettings(boolean openLocationSettings) {
        mOpenLocationSettings = openLocationSettings;
    }

    /* ***
     * PROTECTED METHODS
     */

    public Engine getEngine() {
        return mEngine;
    }

    public ArpiGlView getArpiGlView() {
        return mArpiGlView;
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

    public void setLocationProvider(LocationProvider provider) {
        for (LocationListener l : mLocationProvider.getListeners()) {
            provider.registerListener(l);
        }
        this.mLocationProvider = provider;
    }


    private class NetworkStateBroadcastReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
            if (networkInfo != null && networkInfo.isConnected()) {
                mEngine.updateTileDiffuseMaps();
            }
        }
    }

    private class LocationStateBroadcastReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            if (mLocationTracking) {
                if (mLocationProvider.isGpsLocationAvailable() || mLocationProvider.isNetworkLocationAvailable()) {
                    mLocationProvider.setListeningEnabled(true);
                } else {
                    mLocationProvider.setListeningEnabled(false);
                }
            }
        }
    }
}
