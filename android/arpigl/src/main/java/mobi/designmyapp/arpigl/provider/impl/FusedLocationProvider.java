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

import java.util.LinkedList;
import java.util.List;

import mobi.designmyapp.arpigl.BuildConfig;
import mobi.designmyapp.arpigl.provider.LocationProvider;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;

/**
 * Position provider that use the GPS to get its position. inspired by
 *
 * @author Nicolas THIERION
 */
public final class FusedLocationProvider implements LocationProvider {

    private static final String TAG = FusedLocationProvider.class.getSimpleName();

    /* ***
     * POSITION VALIDITY PARAMETERS
     */
    /**
     * 30 sec.
     */
    private static final int THIRTY_SEC = 1000 * 30 * 1;
    /**
     * time before considering that a position is obsolete.
     */
    private static final int LOCATION_TTL = THIRTY_SEC;                 // ms
    /**
     * we considere a position is acurate if more precise than that distance.
     */
    private static final int LOCATION_ACCURACY = 20;                         // meters

    /* ***
     * PROVIDERS PARAMETERS
     */
    /**
     * NETWORK : The minimum distance to change Updates in meters.
     */
    private static final long NETWORK_MIN_DISTANCE_UPDATE = 0;                          // [meters]
    /**
     * NETWORK : The minimum time between updates in milliseconds.
     */
    private static final long NETWORK_MIN_TIME_UPDATE = 0;                          // [s]
    /**
     * GPS : The minimum distance to change Updates in meters.
     */
    private static final long GPS_MIN_DISTANCE_UPDATE = 0;                          // [meters]
    /**
     * GPS : The minimum time between updates in milliseconds.
     */
    private static final long GPS_MIN_TIME_UPDATE = 0;                           // 1000 * 5 * 1 [ms]

    /* ***
     * ATTRIBUTES
     */
    /**
     * this instance.
     */
    private static FusedLocationProvider mInstance;
    /**
     * application context.
     */
    private final Context mContext;
    /**
     * list of object to notify.
     */
    private final List<LocationListener> mListeners;
    /**
     * Location Manager.
     */
    private final LocationManager mLocationManager;
    /**
     * our locationEvent listener.
     */
    private final LocationListener mLocationlistener = new FusedLocationListener();

    /**
     * current location.
     */
    private Location mCurrentLocation = new Location("");
    private Location mLastKnownLocation;
    /* ***
     * CONSTRUCTORS
     */

    private FusedLocationProvider(Context context) {
        mContext = context.getApplicationContext();

        // init listeners list
        mListeners = new LinkedList<>();

        // get the location manager
        mLocationManager = (LocationManager) mContext.getSystemService(Context.LOCATION_SERVICE);

        // ensure we properly retrieved the location manager before continue.
        if (mLocationManager == null) {
            Log.e(TAG, "cannot get location manager.");
            if (BuildConfig.DEBUG) {
                throw new AssertionError("cannot get location manager.");
            }
            return;
        }

        if (!isNetworkLocationAvailable()) {
            Log.w(TAG, "no network-based location provider available for this device.");
        }

        if (!isGpsLocationAvailable()) {
            Log.w(TAG, "no GPS-based location provider available for this device.");
        }

        // finally, retrieve the fused location from last known position
        mCurrentLocation = mLastKnownLocation = mGetLastKnownFusedLocation();
    }

    /**
     * Get the instance of this position provider.
     *
     * @param context The application context.
     * @return the GPSLocationProvider instance.
     */
    public static FusedLocationProvider getInstance(Context context) {
        if (mInstance == null) {
            synchronized (RotationVectorOrientationProvider.class) {
                if (mInstance == null) {
                    mInstance = new FusedLocationProvider(context);
                }
            }
        }
        return mInstance;
    }

    /* ***
     * GETTERS
     */

    /**
     * Determines whether one Location reading is better than the current
     * Location fix.
     *
     * @param location            The new Location that to evaluate.
     * @param currentBestLocation The current Location fix, to compare with the new one.
     * @return true if the 1st location is better than the second one.
     */
    protected static boolean mIsBestLocation(final Location location,
                                             final Location currentBestLocation) {
        if (currentBestLocation == null) {
            // A new location is always better than no location
            return true;
        }
        if (location == null) {
            // A new location is always better than no location
            return false;
        }

        // Check whether the new location fix is newer or older
        final long timeDelta = location.getTime() - currentBestLocation.getTime();
        final boolean isSignificantlyNewer = timeDelta > LOCATION_TTL;
        final boolean isSignificantlyOlder = timeDelta < -LOCATION_TTL;
        final boolean isNewer = timeDelta > 0;

        // If it's been more than two minutes since the current location, use
        // the new location
        // because the user has likely moved
        if (isSignificantlyNewer) {
            return true;
            // If the new location is more than two minutes older, it must be
            // worse
        } else if (isSignificantlyOlder) {
            return false;
        }

        // Check whether the new location fix is more or less accurate
        final int accuracyDelta = (int) (location.getAccuracy() - currentBestLocation.getAccuracy());
        final boolean isLessAccurate = accuracyDelta > 0;
        final boolean isMoreAccurate = accuracyDelta < 0;
        final boolean isSignificantlyLessAccurate = accuracyDelta > LOCATION_ACCURACY;

        // Check if the old and new location are from the same provider
        final boolean isFromSameProvider = mIsSameProvider(location.getProvider(),
                currentBestLocation.getProvider());

        // Determine location quality using a combination of timeliness and
        // accuracy
        if (isMoreAccurate) {
            return true;
        } else if (isNewer && !isLessAccurate) {
            return true;
        } else if (isNewer && !isSignificantlyLessAccurate && isFromSameProvider) {
            return true;
        }
        return false;
    }

    /**
     * Checks whether two providers are the same.
     */
    private static boolean mIsSameProvider(final String provider1, final String provider2) {
        if (provider1 == null) {
            return provider2 == null;
        }
        return provider1.equals(provider2);
    }

    /**
     * update current location if provided location is better than the old one.
     */
    private static Location mFuseLocation(final Location location1, final Location location2) {
        if (mIsBestLocation(location1, location2)) {
            return location1;
        }
        return location2;
    }

    /**
     * @return true if the network location provider is available.
     */
    public boolean isNetworkLocationAvailable() {
        return mLocationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
    }

    /**
     * @return true if the GPS location provider is available.
     */
    public boolean isGpsLocationAvailable() {
        return mLocationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
    }

    @Override
    public boolean isEnabled() {
        final boolean res = isNetworkLocationAvailable() || isGpsLocationAvailable();
        if (!res) {
            Log.w(TAG,
                    "neither network nor GPS location is available for this device. "
                            + "This PositionProvider cannot work.");
        }
        return res;
    }

    @Override
    public boolean isAvailable() {
        return mContext.getPackageManager().hasSystemFeature(PackageManager.FEATURE_LOCATION);
    }

    /* ***
     * LISTENERS STUFF
     */

    @Override
    public double getLatitude() {
        return mCurrentLocation.getLatitude();
    }

    @Override
    public double getLongitude() {
        return mCurrentLocation.getLongitude();
    }

    @Override
    public Location getLastKnownLocation() {
        return mLastKnownLocation;
    }

    @Override
    public void registerListener(final LocationListener listener) {
        // checks that sensor is available.
        if (!isAvailable()) {
            throw new UnsupportedOperationException(
                    "this device doesn't have the required sensors enabled.");
        }

        synchronized (mListeners) {
            if (!mListeners.contains(listener)) {
                mListeners.add(listener);
            } else {
                return;
            }
            // if listeners just got filled, wake up listening.
            if (mListeners.size() == 1) {

                // wake up network provider
                if (isNetworkLocationAvailable()) {
                    // Register the listener with the NETWORK_PROVIDER
                    // to receive location updates
                    mLocationManager.requestLocationUpdates(
                            LocationManager.NETWORK_PROVIDER,
                            NETWORK_MIN_TIME_UPDATE,
                            NETWORK_MIN_DISTANCE_UPDATE,
                            mLocationlistener);

                    Log.v(TAG, "waking up listening to NETWORK_PROVIDER");
                }

                // wake up GPS provider
                if (isGpsLocationAvailable()) {
                    // Register the listener with the GPS_PROVIDER
                    // to receive location updates
                    mLocationManager.requestLocationUpdates(
                            LocationManager.GPS_PROVIDER,
                            GPS_MIN_TIME_UPDATE, GPS_MIN_DISTANCE_UPDATE,
                            mLocationlistener);
                    Log.v(TAG, "waking up listening to GPS_PROVIDER");

                }
            }
        }
    }

    @Override
    public void unregisterListener(final LocationListener listener) {
        synchronized (mListeners) {

            mListeners.remove(listener);

            // if listeners is empty, wake up listening.
            if (mListeners.isEmpty()) {
                if (BuildConfig.DEBUG && !isAvailable()) {
                    throw new AssertionError(
                            "unregistered listener while Provider was not available.");
                }
                Log.v(TAG, "stop listening to ROTATION_VECTOR");
                mLocationManager.removeUpdates(mLocationlistener);
            }
        }
    }

    /* ***
     * PRIVATE METHODS
     */
    private Location mGetLastKnownFusedLocation() {
        if (!isAvailable()) {
            Log.e(TAG, "Cannot get location.  No location provider is available.");
            return null;
        }
        Location networkLocation = null;
        Location gpsLocation = null;
        Location lastKnownLocation = null;
        // First get location from Network Provider
        if (isNetworkLocationAvailable()) {
            Log.d(TAG, "getting location fix from network");
            networkLocation = mLocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);

            // fuse location.
            lastKnownLocation = mFuseLocation(networkLocation, lastKnownLocation);
        }
        // if GPS Enabled get lat/long using GPS Services
        if (isGpsLocationAvailable()) {
            Log.d(TAG, "getting location from GPS");
            gpsLocation = mLocationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            // fuse location.
            lastKnownLocation = mFuseLocation(gpsLocation, lastKnownLocation);
        }
        if (lastKnownLocation == null) {
            Log.w(TAG, "cannot get location");
            lastKnownLocation = new Location("no location");
        }
        return lastKnownLocation;
    }

    private class FusedLocationListener implements LocationListener {
        @Override
        public void onLocationChanged(Location location) {

            mCurrentLocation = mLastKnownLocation = mFuseLocation(location, mCurrentLocation);
            synchronized (mListeners) {
                for (final LocationListener listener : mListeners) {
                    listener.onLocationChanged(mCurrentLocation);
                }
            }
        }

        @Override
        public void onProviderDisabled(String provider) {
            // simply redispatch the event.
            synchronized (mListeners) {
                for (final LocationListener listener : mListeners) {
                    listener.onProviderDisabled(provider);
                }
            }
        }

        @Override
        public void onProviderEnabled(String provider) {
            // simply redispatch the event.
            synchronized (mListeners) {
                for (final LocationListener listener : mListeners) {
                    listener.onProviderEnabled(provider);
                }
            }
        }

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {

            // simply redispatch the event.
            synchronized (mListeners) {
                for (final LocationListener listener : mListeners) {
                    listener.onStatusChanged(provider, status, extras);
                }
            }
        }
    }

}

