package mobi.designmyapp.arpigl.listener;

import android.location.Location;
import android.location.LocationListener;
import android.os.Bundle;

import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

/**
 * Created by Loïc Ortola on 27/08/2015.
 */
public class MultiLocationListener implements LocationListener {

    private final Set<LocationListener> mListeners = Collections.synchronizedSet(new HashSet<LocationListener>());

    // To implement change checks
    protected boolean shouldNotify(Location location) {
        return true;
    }

    @Override
    public void onLocationChanged(Location location) {
        if (shouldNotify(location)) {
            for (LocationListener listener : mListeners) {
                listener.onLocationChanged(location);
            }
        }
    }

    @Override
    public void onProviderDisabled(String provider) {
        for (LocationListener listener : mListeners) {
            listener.onProviderDisabled(provider);
        }
    }

    @Override
    public void onProviderEnabled(String provider) {
        for (LocationListener listener : mListeners) {
            listener.onProviderEnabled(provider);
        }
    }

    @Override
    public void onStatusChanged(String provider, int status, Bundle extras) {
        for (LocationListener listener : mListeners) {
            listener.onStatusChanged(provider, status, extras);
        }
    }

    public void addListener(LocationListener listener) {
        mListeners.add(listener);
    }

    public void removeListener(LocationListener listener) {
        mListeners.remove(listener);
    }

    public Collection<LocationListener> getListeners() {
        return Collections.unmodifiableCollection(mListeners);
    }
}
