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

package mobi.designmyapp.arpigl.engine;

import mobi.designmyapp.arpigl.listener.EngineListener;
import mobi.designmyapp.arpigl.model.Poi;

/**
 * Java interface for native cpp class 'dma::geo::GeoEngine'. Adds lifecycle
 * control capabilities over stock {@code #IEngineController}.
 *
 * @author Nicolas THIERION
 * @version 0.2.3 - Changelog : removed exit codes in return values.
 */
public interface EngineBridge extends EngineController {

    /**
     * Initialize the engine. Be careful to have a valid openGL context opened &
     * binded before calling this method. Otherwise, the method will fail. Be
     * carefull to call {@link #setSurfaceSize(int, int)} to create a valid
     * drawing surface.
     *
     * @return true if initialized properly, false otherwise.
     */
    boolean init();

    /**
     * Destroys the engine, and free all memory on CPP side.
     */
    void destroy();

    /**
     * Refreshes all OpenGL resources. The OpenGL context needs to be refreshed
     * after a call to {@link #wipe}. If the engine steps on a
     * not-refreshed context, exceptions will happen.
     */
    void refresh();

    /**
     * Clean all GPU resources.
     */
    void wipe();

    /**
     * Clean this engine.
     */
    void unload();

    /**
     * Steps the engine.
     */
    void step();

    /**
     * Notify the engine that a tile is available.
     *
     * @param x the tile x
     * @param y the tile y
     * @param z the tile z
     */
    void notifyTileAvailable(int x, int y, int z);

    /* ***
     * GETTERS
     */

    /**
     * Checks if is inits the.
     *
     * @return true if engine is initialized
     */
    boolean isInit();

    /**
     * Checks if is able to draw.
     *
     * @return true if the Engine is able to draw (it is initialized, and has a
     * valid openGL context.)
     */
    boolean isAbleToDraw();

    /**
     * Checks for poi.
     *
     * @param poi the poi
     * @return true if the entity belongs to the scene.
     */
    boolean hasPoi(Poi poi);

    /* ***
     * SETTERS
     */

    /**
     * Set the viewport size. Typically used when viewport size changes.
     *
     * @param width  required width of the viewport.
     * @param height required height of the viewport.
     */
    void setSurfaceSize(final int width, final int height);

    /**
     * Sets the {@link EngineListener} to be notified on engine event
     * thrown by native implementation.
     *
     * @param callbacks the engine callback
     */
    void setNativeCallbacks(EngineListener callbacks);

}
