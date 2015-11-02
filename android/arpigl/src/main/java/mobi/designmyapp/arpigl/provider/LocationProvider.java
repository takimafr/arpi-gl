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

package mobi.designmyapp.arpigl.provider;

import android.location.Location;
import android.location.LocationListener;

import java.util.Collection;
import java.util.List;

/**
 * Provides a location.
 */
public interface LocationProvider {

    /**
     * @return true if this provider is available for this device.
     */
    boolean isAvailable();

    /**
     * @return true if this provider is currently listening.
     */
    boolean isListening();

    /**
     * Add the given listener to be notified when position changes. Should
     * typically be called in Activity's onResume() method.
     *
     * @param listener listener to register
     */
    void registerListener(LocationListener listener);

    /**
     * Remove the given listener from active listeners. Will not be notified
     * anymore when position changes. Should typically be called in Activity's
     * onPause() method.
     *
     * @param listener listener to unregister
     */
    void unregisterListener(LocationListener listener);

    /**
     * Retrieve available listeners.
     * @return the list of listeners
     */
    Collection<LocationListener> getListeners();

    /**
     * Enable listening to new locations
     * @param enabled boolean value
     */
    void setListeningEnabled(boolean enabled);

    /**
     * @return the last known location.
     */
    Location getLastKnownLocation();

}
