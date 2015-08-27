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

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import mobi.designmyapp.arpigl.event.TileEvent;
import mobi.designmyapp.arpigl.listener.TileEventListener;
import mobi.designmyapp.arpigl.model.Tile;

public abstract class TileProvider extends Provider<Tile.Id, TileEvent, TileEventListener> {

    Queue<TileEvent> eventsOnHold = new ConcurrentLinkedQueue<>();

    /* ***
     * CONSTRUCTOR
     */
    public TileProvider(String uri) {
        super(uri);
    }

    @Override
    protected void postEvent(TileEvent event) {
        if (mEventBus.hasSubscriberForEvent(TileEvent.class)) {
            super.postEvent(event);
        } else {
            eventsOnHold.add(event);
        }
    }

    @Override
    public void register(TileEventListener listener) {
        super.register(listener);
        if (!eventsOnHold.isEmpty()) {
            TileEvent event;
            while ((event = eventsOnHold.poll()) != null) {
                mEventBus.post(event);
            }
        }
    }
}
