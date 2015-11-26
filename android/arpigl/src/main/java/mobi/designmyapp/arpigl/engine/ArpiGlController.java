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

import android.app.Activity;
import android.app.Application.ActivityLifecycleCallbacks;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.SensorEvent;
import android.location.Location;
import android.location.LocationListener;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;
import android.widget.Toast;

import java.lang.ref.WeakReference;
import java.util.LinkedList;
import java.util.List;

import mobi.designmyapp.arpigl.event.PoiEvent;
import mobi.designmyapp.arpigl.event.TileEvent;
import mobi.designmyapp.arpigl.listener.EngineListener;
import mobi.designmyapp.arpigl.listener.OrientationListener;
import mobi.designmyapp.arpigl.listener.PoiEventListener;
import mobi.designmyapp.arpigl.listener.PoiSelectionListener;
import mobi.designmyapp.arpigl.listener.TileEventListener;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Tile;
import mobi.designmyapp.arpigl.provider.OrientationProvider;
import mobi.designmyapp.arpigl.provider.PoiProvider;
import mobi.designmyapp.arpigl.provider.TileProvider;
import mobi.designmyapp.arpigl.provider.impl.RotationVectorOrientationProvider;
import mobi.designmyapp.arpigl.ui.ArpiGlFragment;
import mobi.designmyapp.arpigl.util.InternalBus;
import mobi.designmyapp.arpigl.util.ProjectionUtils;
import mobi.designmyapp.arpigl.util.TileCache;

/**
 * Controls the DMA engine. Allows to add or remove some POIs.
 *
 * @author Nicolas THIERION.
 */
public final class ArpiGlController implements Controller {


    /* ***
     * CONSTANTS
     */
    /**
     * Zoom level of tiles displayed on the 3D view.
     */
    public static final int MAP_ZOOM = 19;

    private static final float MIN_ALTITUDE = 1.0f;
    private static final float MAX_ALTITUDE = 15.0f;

    /**
     * debug tag.
     */
    private static final String TAG = ArpiGlController.class.getSimpleName();
    private static final int TILE_CACHE_SIZE = 250;

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
    private final OrientationListener mOrientationListener = new ControllerOrientationListener();
    /* ***
     * PROVIDERS
     */
    private final LocationListener mLocationListener = new ControllerLocationListener();
    /* ***
     * LOCKS
     */
    private final Object mLock;

    private int mLastX;
    private int mLastY;
    private boolean mLoaded;
    private TileCache mTileCache;

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
    private List<PoiProvider<?>> mPoiProviders = new LinkedList<>();

    private InternalBus mEventBus;
    private ScaleGestureDetector mScaleGestureDetector;
    private GestureDetector mGestureDetector;

    /* ***
     * LISTENERS
     */
    private TileEventListener mTileEventListener = new ControllerTileEventListener();
    private EngineListener mEngineCallbacks = new ControllerEngineListener();
    private PoiEventListener mPoiEventListener = new ControllerPoiEventListener();
    private PoiSelectionListener mPoiSelectionListener;
    /* ***
         * ATTRIBUTES
         */
    private Engine mEngine;

    /* ***
     * CONSTRUCTORS
     */

    /**
     * Creates a new controller to control the engine of given fragment.
     *
     * @param fragment the fragment to control.
     */
    public ArpiGlController(ArpiGlFragment fragment) {
        mFragment = fragment;
        mLock = new Object();
        mEngine = mFragment.getEngine();

        Activity activity = mFragment.getActivity();
        mActivityContext = new WeakReference<>(activity);

        // init OrientationProvider
        mOrientationProvider = RotationVectorOrientationProvider.getInstance(activity);

        // register callback to be notified when the native engine needs us to
        // fetch new tiles.
        mEngine.setNativeEngineListener(mEngineCallbacks);

        activity.getApplication().registerActivityLifecycleCallbacks(mActivityLifecycleCallbacks);
        mActivityLifecycleCallbacks.onActivityResumed(activity);

        mEventBus = InternalBus.getInstance();

        // setup gesture detectors
        mGestureDetector = new GestureDetector(mActivityContext.get(), new GestureDetector.SimpleOnGestureListener() {
            @Override
            public boolean onSingleTapUp(MotionEvent e) {
                mEngine.selectPoi((int) e.getX(), (int) e.getY());
                return true;
            }

            @Override
            public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
                double[] cameraPosition = mEngine.getCameraPosition();
                float offset = - distanceY * 0.02f;
                double alt = cameraPosition[2] + offset;
                setCameraPosition(cameraPosition[0], cameraPosition[1], Math.min(Math.max(MIN_ALTITUDE, alt), MAX_ALTITUDE), false);
                return true;
            }
        });

        // handle scale gesture for zoom
        mScaleGestureDetector = new ScaleGestureDetector(mActivityContext.get(), new ScaleGestureDetector.SimpleOnScaleGestureListener() {
            @Override
            public boolean onScale(ScaleGestureDetector detector) {
                float scale = detector.getScaleFactor();
                mEngine.zoom(scale - 1.0f);
                return true;
            }
        });

        mFragment.getArpiGlView().setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                mScaleGestureDetector.onTouchEvent(motionEvent);
                mGestureDetector.onTouchEvent(motionEvent);
                return true;
            }
        });
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
        synchronized (mLock) {
            if (tileProvider == mTileProvider) {
                return;
            }

            // stop listening previous provider
            if (mTileProvider != null) {
                mTileProvider.unregister(mTileEventListener);
            }

            mTileProvider = tileProvider;
            if (tileProvider != null) {
                String namespace = tileProvider.getNamespace();
                mTileCache = new TileCache(mEngine.getInstallationDir() + "/texture/tiles/" + namespace + "/", "png", TILE_CACHE_SIZE);
                mEngine.setTileNamespace(namespace);
                // listen to given provider
                tileProvider.register(mTileEventListener);
            } else {
                mTileCache = null;
                mEngine.setTileNamespace("");
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
        synchronized (mLock) {
            if (poiProvider != null && !mPoiProviders.contains(poiProvider)) {
                mPoiProviders.add(poiProvider);

                // listen to given provider
                mEventBus.register(mPoiEventListener);
            }
        }
    }

    /**
     * Removes the given poi provider.
     *
     * @param poiProvider the poi provider to remove.
     */
    public void removePoiProvider(PoiProvider<?> poiProvider) {
        synchronized (mLock) {
            // stop listening the provider
            mEventBus.unregister(mPoiEventListener);
            mPoiProviders.remove(poiProvider);
        }
    }

    /**
     * Sets the poi selection listener.
     * @param poiSelectionListener the poi selection listener
     */
    public void setPoiSelectionListener(PoiSelectionListener poiSelectionListener) {
        synchronized (mLock) {
            mPoiSelectionListener = poiSelectionListener;
        }
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
        setCameraPosition(lat, lon, mEngine.getCameraPosition()[2]);
    }

    @Override
    public void setCameraPosition(double lat, double lng, double alt) {
        setCameraPosition(lat, lng, alt, true);
    }

    @Override
    public void setCameraPosition(double lat, double lon, double alt, boolean animated) {
        mEngine.setCameraPosition(lat, lon, alt, animated);

        int x = ProjectionUtils.lng2tilex(lon, MAP_ZOOM);
        int y = ProjectionUtils.lat2tiley(lat, MAP_ZOOM);

        synchronized (mLock) {
            if (mLastX == x && mLastY == y && mLoaded) {
                return;
            }

            Tile.Id tile = new Tile.Id(x, y, MAP_ZOOM);
            // Our tile exists, move to head of queue
            if (mTileCache != null && mTileCache.contains(tile)) {
                mTileCache.moveLast(tile);
            }

            // Warning: this calls all poi providers each time the camera is moved. This means caching should be implemented on PoiProviders
            for (PoiProvider provider : mPoiProviders) {
                if (provider != null) {
                    mLoaded = false;
                    provider.fetch(new Tile.Id(x, y, MAP_ZOOM));
                }
            }

            mLastX = x;
            mLastY = y;
        }
    }

    @Override
    public final boolean isInstalled() {
        return mEngine.isInstalled();
    }

    @Override
    public final void addPoi(Poi poi) {
        mEngine.addPoi(poi);
    }

    @Override
    public final void removePoi(Poi poi) throws IllegalStateException {
        mEngine.removePoi(poi);
    }

    @Override
    public void setPoiPosition(String sid, double lat, double lng, double alt) {
        mEngine.setPoiPosition(sid, lat, lng, alt);
    }

    @Override
    public void setPoiColor(String sid, int color) {
        mEngine.setPoiColor(sid, color);
    }

    @Override
    public final void setOrigin(double latitude, double longitude) {
        mEngine.setOrigin(latitude, longitude);
    }

    @Override
    public void setSkyBox(String sid) {
        mEngine.setSkyBox(sid);
    }

    @Override
    public void setSkyBoxEnabled(boolean enabled) {
        mEngine.setSkyBoxEnabled(enabled);
    }


    @Override
    public final void setCameraRotation(float[] rotationMatrix) {
        mEngine.setCameraRotation(rotationMatrix);
    }

    public void setOpenLocationSettings(boolean openLocationSettings) {
        mFragment.setOpenLocationSettings(openLocationSettings);
    }

    /* ***
     * PRIVATE CLASSES
     */
    private class ActivityCallbacks implements ActivityLifecycleCallbacks {

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

            mFragment.getLocationProvider().registerListener(mLocationListener);
            mOrientationProvider.registerListener(mOrientationListener);

            if (mTileProvider != null) {
                mEventBus.register(mTileEventListener);
            }

            for (PoiProvider provider : mPoiProviders) {
                if (provider != null) {
                    mEventBus.register(mPoiEventListener);
                }
            }
        }

        @Override
        public void onActivityPaused(Activity activity) {
            if (!activity.equals(mActivityContext.get())) {
                return;
            }

            mOrientationProvider.unregisterListener(mOrientationListener);
            mFragment.getLocationProvider().unregisterListener(mLocationListener);

            if (mTileProvider != null) {
                mEventBus.unregister(mTileEventListener);
            }

            for (PoiProvider provider : mPoiProviders) {
                if (provider != null) {
                    mEventBus.unregister(mPoiEventListener);
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

            if (mTileCache != null) {
                mTileCache.put(tile.getId(), tile.getData());
            }

            // tell the engine that it can now load this tile.
            mEngine.notifyTileAvailable(id.x, id.y, id.z);
        }
    }

    private class ControllerPoiEventListener implements PoiEventListener {
        @Override
        public void onEvent(PoiEvent event) {
            mLoaded = true;
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
    private class ControllerEngineListener implements EngineListener {

        @Override
        public void onTileRequest(int x, int y, int z) {
            synchronized (mLock) {
                Tile.Id tid = new Tile.Id(x, y, z);
                // fetch the requested tile
                fetchTile(tid);
            }
        }

        @Override
        public void onPoiSelected(final String sid) {
            synchronized (mLock) {
                if (mPoiSelectionListener != null) {
                    mPoiSelectionListener.onPoiSelected(sid);
                }
            }
        }

        @Override
        public void onPoiDeselected(final String sid) {
            synchronized (mLock) {
                if (mPoiSelectionListener != null) {
                    mPoiSelectionListener.onPoiDeselected(sid);
                }
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
