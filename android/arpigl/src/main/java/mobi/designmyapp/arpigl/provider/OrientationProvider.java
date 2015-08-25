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

import mobi.designmyapp.arpigl.listener.OrientationListener;

/**
 * Provides an orientation.
 */
public interface OrientationProvider {

    /**
     * @return true if this provider is available for this device
     */
    boolean isAvailable();

    /**
     * @return true if the provider got some new values.
     */
    boolean hasNewValue();

    /**
     * @return the current rotation matrix.
     */
    float[] getRotationMatrix();

    /**
     * Add the given listener to be notified when orientation changes. Should
     * typically be called in Activity's onResume() method.
     *
     * @param listener listener to register
     */
    void registerListener(OrientationListener listener);

    /**
     * Remove the given listener from active listeners. Will not be notified
     * anymore when orientation changes. Should typically be called in
     * Activity's onPause() method.
     *
     * @param listener listener to unregister
     */
    void unregisterListener(OrientationListener listener);

}
