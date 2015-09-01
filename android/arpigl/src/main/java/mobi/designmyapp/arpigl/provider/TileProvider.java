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
import mobi.designmyapp.arpigl.util.InternalBus;

public abstract class TileProvider {

    Queue<TileEvent> eventsOnHold = new ConcurrentLinkedQueue<>();
    InternalBus mEventBus;
    /**
     * Default Constructor.
     */
    public TileProvider() {
        mEventBus = InternalBus.getInstance();
    }

    protected void postEvent(TileEvent event) {
        if (mEventBus.hasObservers(TileEvent.class)) {
            mEventBus.post(event);
        } else {
            eventsOnHold.add(event);
        }
    }

    public abstract void fetch(Tile.Id tid);

    public void register(TileEventListener listener) {
        mEventBus.register(listener);
        if (!eventsOnHold.isEmpty()) {
            TileEvent event;
            while ((event = eventsOnHold.poll()) != null) {
                mEventBus.post(event);
            }
        }
    }

    public void unregister(TileEventListener listener) {
        mEventBus.unregister(listener);
    }
}
