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

import java.lang.ref.WeakReference;
import java.util.LinkedList;
import java.util.List;

import mobi.designmyapp.arpigl.engine.Engine;
import mobi.designmyapp.arpigl.engine.EngineController;
import mobi.designmyapp.arpigl.engine.TileMapCache;
import mobi.designmyapp.arpigl.event.PoiEvent;
import mobi.designmyapp.arpigl.event.TileEvent;
import mobi.designmyapp.arpigl.listener.EngineListener;
import mobi.designmyapp.arpigl.listener.OrientationListener;
import mobi.designmyapp.arpigl.listener.PoiEventListener;
import mobi.designmyapp.arpigl.listener.TileEventListener;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.LocationProvider;
import mobi.designmyapp.arpigl.provider.OrientationProvider;
import mobi.designmyapp.arpigl.provider.PoiProvider;
import mobi.designmyapp.arpigl.provider.TileProvider;
import mobi.designmyapp.arpigl.provider.impl.RotationVectorOrientationProvider;
import mobi.designmyapp.arpigl.ui.ArpiGlFragment;
import mobi.designmyapp.arpigl.util.ProjectionUtils;
import android.app.Activity;
import android.app.Application;
import android.app.Application.ActivityLifecycleCallbacks;
import android.hardware.SensorEvent;
import android.location.Location;
import android.location.LocationListener;
import android.os.Bundle;
import android.util.Log;

/**
 * Controls the DMA engine. Allows to add or remove some POIs.
 *
 * @author Nicolas THIERION.
 */
public final class ArpiGlController extends AbstractEngineController implements EngineController {

    /* ***
     * CONSTANTS
     */
    /**
     * Zoom level of tiles diplayed on the 3D view.
     */
    public static final int MAP_ZOOM = 19;

    public static final double DEFAULT_ALTITUDE = 5.0;

    /**
     * debug tag.
     */
    private static final String TAG = ArpiGlController.class.getSimpleName();
    private static final int TILE_CACHE_SIZE = 125;

    /* ***
     * ATTRIBUTES
     */
    /**
     * the MapGLFragment controlled by this controller.
     */
    private final ArpiGlFragment mFragment;
    /**
     * The activity context, used to register our Activity-callbacks.
     */
    private final WeakReference<Activity> mActivityContext;
    /**
     * We use this callbacks to watch activity's lifecycle, and connect /
     * disconnect listeners & providers on resume / pause.
     */
    private final ActivityLifecycleCallbacks mActivityLifecycleCallbacks = new ActivityCallbacks();
    /**
     * delegates operations to this controller.
     */
    private final Engine mEngine;
    private final OrientationListener mOrientationListener = new ControllerOrientationListener();

    private int mLastX;
    private int mLastY;

    /* ***
     * PROVIDERS
     */
    private final LocationListener mLocationListener = new ControllerLocationListener();
    /* ***
     * LOCKS
     */
    private final Object sProvider;
    private TileMapCache mTileMap;
    /**
     * we get device location from this provider.
     */
    private LocationProvider mLocationProvider;
    /**
     * we get device orientation from this provider.
     */
    private OrientationProvider mOrientationProvider;
    /**
     * we fetch tiles from this provider to feed the Engine.
     */
    private TileProvider mTileProvider;
    /**
     * we fetch pois from this provider to feed the Engine.
     */
    private List<PoiProvider<?>> mPoiProvider = new LinkedList<>();
    /* ***
     * LISTENERS
     */
    private TileEventListener mTileEventListener = new ControllerTileEventListener();
    private EngineListener mEngineCallbacks = new ControllerEngineListener();
    private PoiEventListener mPoiEventListener = new ControllerPoiEventListener();

    /* ***
     * CONSTRUCTORS
     */

    /**
     * Creates a new controller to control the engine of given fragment.
     *
     * @param fragment the fragment to control.
     */
    public ArpiGlController(ArpiGlFragment fragment) {
        super(fragment.getEngine());
        mFragment = fragment;
        sProvider = new Object();
        mEngine = mFragment.getEngine();

        Activity activity = mFragment.getActivity();
        mActivityContext = new WeakReference<>(activity);
        activity.getApplication().registerActivityLifecycleCallbacks(mActivityLifecycleCallbacks);

        // init location provider
        mLocationProvider = mFragment.getLocationProvider();
        mFragment.addLocationListener(mLocationListener);
        mTileMap = new TileMapCache(mEngine.getInstallationDir() + "/texture/tiles/provided/", TILE_CACHE_SIZE);

        // init OrientationProvider
        mOrientationProvider = RotationVectorOrientationProvider.getInstance(activity);

        // register callback to be notified when the native engine needs us to
        // fetch new tiles.
        mEngine.setNativeCallbacks(mEngineCallbacks);

        setCameraPosition(mLocationProvider.getLatitude(), mLocationProvider.getLongitude(), DEFAULT_ALTITUDE);

    }

    /* ***
     * PUBLIC METHODS
     */

    /**
     * Sets the tile provider. The given {@link TileProvider} will automatically
     * feed this {@link ArpiGlController}.
     *
     * @param tileProvider the new tile provider
     */
    public void setTileProvider(TileProvider tileProvider) {
        synchronized (sProvider) {
            if (tileProvider == mTileProvider) {
                return;
            }

            // stop listening previous provider
            if (mTileProvider != null) {
                mTileProvider.unregister(mTileEventListener);
            }

            mTileProvider = tileProvider;
            if (tileProvider != null) {

                // listen to given provider
                tileProvider.register(mTileEventListener);
            }
        }
    }

    /**
     * Adds the poi provider. The given {@link PoiProvider} will automatically
     * feed this {@link ArpiGlController}.
     *
     * @param poiProvider the new poi provider
     */
    public void addPoiProvider(PoiProvider<?> poiProvider) {
        synchronized (sProvider) {
            if (poiProvider == mPoiProvider) {
                return;
            }

            if (poiProvider != null && !mPoiProvider.contains(poiProvider)) {
                mPoiProvider.add(poiProvider);

                // listen to given provider
                poiProvider.register(mPoiEventListener);
            }
        }
    }

    /**
     * Removes the given poi provider.
     *
     * @param poiProvider the poi provider to remove.
     */
    public void removePoiProvider(PoiProvider<?> poiProvider) {
        synchronized (sProvider) {
            // stop listening the provider
            poiProvider.unregister(mPoiEventListener);
            mPoiProvider.remove(poiProvider);
        }
    }

    /**
     * Gets the location provider.
     *
     * @return the location provider
     */
    public LocationProvider getLocationProvider() {
        return mLocationProvider;
    }

    /**
     * Enable or disable Location tracking. When enabled, the view will be
     * updated when device's location changes.
     *
     * @param locationTracking true if should enable location tracking.
     */
    public void setUserLocationEnabled(boolean locationTracking) {
        mFragment.setUserLocationEnabled(locationTracking);
    }

    @Override
    public void setCameraPosition(double lat, double lon) {
        setCameraPosition(lat, lon, getCameraPosition()[2]);
    }

    @Override
    public void setCameraPosition(double lat, double lon, double alt) {
        super.setCameraPosition(lat, lon, alt);

        int x = ProjectionUtils.lng2tilex(lon, MAP_ZOOM);
        int y = ProjectionUtils.lat2tiley(lat, MAP_ZOOM);

        synchronized (sProvider) {
            if (mLastX == x && mLastY == y) {
                return;
            }

            if (mTileProvider != null) {
                mTileMap.updateCache(x, y, MAP_ZOOM);
            }
            for (PoiProvider provider : mPoiProvider) {
                if (provider != null) {
                    provider.fetch(new Tile.Id(x, y, MAP_ZOOM));
                }
            }

            mLastX = x;
            mLastY = y;
        }
    }

    /* ***
     * PRIVATE CLASSES
     */
    private class ActivityCallbacks implements Application.ActivityLifecycleCallbacks {

        @Override
        public void onActivityCreated(Activity activity, Bundle savedInstanceState) {
        }

        @Override
        public void onActivityStarted(Activity activity) {
        }

        @Override
        public void onActivityResumed(Activity activity) {
            if (!activity.equals(mFragment.getActivity())) {
                return;
            }

            mFragment.addLocationListener(mLocationListener);
            mOrientationProvider.registerListener(mOrientationListener);

            if (mTileProvider != null) {
                mTileProvider.register(mTileEventListener);
            }

            for (PoiProvider provider : mPoiProvider) {
                if (provider != null) {
                    provider.register(mPoiEventListener);
                }
            }
        }

        @Override
        public void onActivityPaused(Activity activity) {
            if (!activity.equals(mActivityContext.get())) {
                return;
            }

            mOrientationProvider.unregisterListener(mOrientationListener);
            mFragment.removeLocationListener(mLocationListener);

            if (mTileProvider != null) {
                mTileProvider.unregister(mTileEventListener);
            }

            for (PoiProvider provider : mPoiProvider) {
                if (provider != null) {
                    provider.unregister(mPoiEventListener);
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
            Log.v(TAG, "onActivityDestroy: unregister callbacks from activity");
            mActivityContext.get().getApplication().unregisterActivityLifecycleCallbacks(mActivityLifecycleCallbacks);
        }
    }

    private class ControllerTileEventListener implements TileEventListener {
        @Override
        public void onEvent(TileEvent event) {
            // put the received tile in cache.
            Tile tile = event.tile;
            Tile.Id id = tile.getId();
            mTileMap.add(tile);

            // tell the engine that it can now load this tile.
            mEngine.notifyTileAvailable(id.x, id.y, id.z);
        }
    }

    private class ControllerPoiEventListener implements PoiEventListener {
        @Override
        public void onEvent(PoiEvent event) {
            // tell the engine that it can now load these pois.
            for (Poi poi : event.getPois()) {
                mEngine.addPoi(poi);
            }
        }
    }


    /**
     * Receive callbacks from native engine. Use the active {@link TileProvider}
     * to fetch required tiles.
     *
     * @author Nicolas THIERION
     */
    private class ControllerEngineListener extends AbstractEngineListener {

        @Override
        public void onTileRequest(int x, int y, int z) {
            synchronized (sProvider) {
                Tile.Id tid = new Tile.Id(x, y, z);
                // fetch the requested tile
                fetchTile(tid);
            }
        }

        private void fetchTile(Tile.Id tid) {
            if (mTileProvider != null) {
                mTileProvider.fetch(tid);
            } else {
                Log.w(TAG, "requesting tiles, but no tile provider was set.");
            }
        }


    }

    private class ControllerOrientationListener implements OrientationListener {
        @Override
        public void onOrientationChanged(SensorEvent event) {
            mEngine.setCameraRotation(mOrientationProvider.getRotationMatrix());
        }
    }

    /**
     * updates camera position according to the new incoming location.
     *
     * @author Nicolas THIERION
     */
    private class ControllerLocationListener implements LocationListener {

        @Override
        public void onLocationChanged(Location location) {

            // update camera position
            double lat = location.getLatitude();
            double lng = location.getLongitude();
            setCameraPosition(lat, lng);
        }

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {
        }

        @Override
        public void onProviderEnabled(String provider) {
        }

        @Override
        public void onProviderDisabled(String provider) {
        }

    }

}
