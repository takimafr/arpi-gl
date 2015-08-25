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

package mobi.designmyapp.arpigl.event;

import java.util.Collection;

import mobi.designmyapp.arpigl.model.Poi;

/**
 * Event to notify when {@link Poi} objects are created.
 *
 * @author Nicolas THIERION.
 */
public class PoiEvent {

    /**
     * the poi collection.
     */
    private Collection<Poi> mPois;

    /**
     * Instantiates a new poi event, that holds the given created pois.
     *
     * @param pois the created pois
     */
    public PoiEvent(Collection<Poi> pois) {
        mPois = pois;

    }

    /**
     * Gets the created pois.
     *
     * @return the created pois
     */
    public final Collection<Poi> getPois() {
        return mPois;
    }

}
