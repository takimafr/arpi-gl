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

package mobi.designmyapp.arpigl.mapper;

import mobi.designmyapp.arpigl.model.Poi;

/**
 * A {@link Mapper} can convert a source of certain type to another
 *
 * @param <T> the return type.
 * @param <U> the datasource type.
 * @author Nicolas THIERION.
 */
public interface Mapper<T, U> {

    /**
     * @param source Source describing an input
     * @return the list of {@link Poi} that matches the Pois
     */
    T convert(U source);


}
