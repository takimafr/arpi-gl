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


import android.opengl.GLSurfaceView;

/**
 * Java interface for native cpp class 'dma::geo::Engine'.
 *
 * @author Nicolas THIERION
 * @version 0.2.3 - Changelog : removed exit codes in return values.
 */
public interface Engine extends EngineBridge {

    /**
     * get the GLSurfaceView.Renderer that uses this Engine as Rendering engine.
     *
     * @return the renderer
     */
    GLSurfaceView.Renderer getRenderer();

    /**
     * Get the absolute path where the engine is installed. (the res folder).
     *
     * @return the engine's installation path.
     */
    String getInstallationDir();

    /**
     * Pauses the engine & disconnect the bridge.
     */
    void pause();

    /**
     * Resume the engine, & reconnect the bridge.
     */
    void resume();

}
