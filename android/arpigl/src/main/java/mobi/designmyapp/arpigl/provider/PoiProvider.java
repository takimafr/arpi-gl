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

import java.util.HashSet;
import java.util.Set;

import mobi.designmyapp.arpigl.event.PoiEvent;
import mobi.designmyapp.arpigl.exception.PoiMapperException;
import mobi.designmyapp.arpigl.listener.PoiEventListener;
import mobi.designmyapp.arpigl.mapper.PoiMapper;
import mobi.designmyapp.arpigl.model.Poi;
import mobi.designmyapp.arpigl.model.Tile.Id;

/**
 * A PoiProvider provide some {@link Poi} out of a
 * datasource, to feed the MapGL engine.
 *
 * @param <T> the datasource type.
 * @author Nicolas THIERION.
 */
public abstract class PoiProvider<T> extends Provider<Id, PoiEvent, PoiEventListener> {

    /**
     * The poi mapper.
     */
    private Class<? extends PoiMapper<T>> mPoiMapper;

    /* ***
     * CONSTRUCTOR
     */

    /**
     * Create a new {@link PoiProvider}, given the {@link PoiMapper} to use to
     * create pois.
     *
     * @param uri            Uri from where to fetch pois.
     * @param poiMapperClass the {@link PoiMapper} class.
     */
    public PoiProvider(String uri, Class<? extends PoiMapper<T>> poiMapperClass) {
        super(uri);
        mPoiMapper = poiMapperClass;
    }

    /**
     * Create a new {@link PoiProvider}, given the {@link PoiMapper} to use to
     * create pois.
     *
     * @param uri Uri from where to fetch pois.
     */
    public PoiProvider(String uri) {
        super(uri);
    }

    /* ***
     * ABSTRACT METHODS
     */

    @Override
    public abstract void fetch(Id tid);

    /* ***
     * PUBLIC METHODS
     */

    protected final Set<Poi> convert(T datasource) throws PoiMapperException {
        Set<Poi> res = new HashSet<>();
        try {
            res.addAll(mPoiMapper.newInstance().convert(datasource));
        } catch (InstantiationException e) {
            throw new IllegalStateException("Error: mapper class not well formed", e);
        } catch (IllegalAccessException e) {
            throw new IllegalStateException("Error: mapper class cannot be accessed (Maybe it is not public?)", e);
        }
        return res;
    }

}
