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

package mobi.designmyapp.arpigl.exception;

import mobi.designmyapp.arpigl.mapper.PoiMapper;
import mobi.designmyapp.arpigl.model.Poi;

/**
 * A PoiMapperException is thrown when an error occurs while mapping a
 * {@link Poi} from a source with a {@link PoiMapper}.
 *
 * @author Nicolas THIERION.
 */
public class PoiMapperException extends Exception {

    private static final long serialVersionUID = 6055666062139338697L;
    private Exception mException;

    /**
     * Create a new PoiMapperException, that holds the error of the exception
     * given in parameter.
     *
     * @param nestedException the nested exception.
     */
    public PoiMapperException(Exception nestedException) {
        super(nestedException);
        mException = nestedException;
    }

    /**
     * Create a new poi mapper exception.
     *
     * @param message the exception message
     */
    public PoiMapperException(String message) {
        super(message);
    }

}
