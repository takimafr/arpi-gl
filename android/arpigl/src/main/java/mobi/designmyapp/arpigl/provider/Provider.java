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

import de.greenrobot.event.EventBus;

/**
 * The Class Provider.
 *
 * @param <I> the Input source type
 * @param <E> the thrown Event type
 * @param <L> the Listener-of-throw-event type
 */
public abstract class Provider<I, E, L> {

    private final String mUri;
    /* ***
     * ATTRIBUTES
     */
    private EventBus mEventBus;

    /* ***
     * CONSTRUCTOR
     */

    /**
     * Instantiates a new provider.
     *
     * @param uri the uri of datasource that feed this provider.
     */
    public Provider(final String uri) {
        mUri = uri;
        mEventBus = new EventBus();
    }

    /* ***
     * ABSTRACT METHODS
     */

    /**
     * Fetch data from datasource. This should be executed asynchronously. WHen
     * finished, this method should post an event of type <E>, to listeners of
     * type <L> registered with the {@code register()} method.
     *
     * @param fetchId the fetch id
     */
    public abstract void fetch(I fetchId);

    /* ***
     * PUBLIC METHODS
     */

    /**
     * Gets the uri of this provider.
     *
     * @return the provider's uri
     */
    public final String getUri() {
        return mUri;
    }

    /**
     * Remember to call this after fetch is done.
     *
     * @param event the event to post
     */
    protected final void postEvent(E event) {
        mEventBus.post(event);
    }

    /**
     * Register the given listener, to receive events thrown by {@link #fetch}
     * method.
     *
     * @param listener the listener to register
     */
    public final void register(L listener) {
        if (!mEventBus.isRegistered(listener)) {
            mEventBus.register(listener);
        }
    }

    /**
     * Unregister the given listener, to stop receiving events thrown by {@link
     * #fetch} method.
     *
     * @param listener the listener to unregister
     */
    public final void unregister(L listener) {
        if (mEventBus.isRegistered(listener)) {
            mEventBus.unregister(listener);
        }
    }
}
