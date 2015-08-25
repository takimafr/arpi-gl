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

package mobi.designmyapp.arpigl.engine.impl;

import mobi.designmyapp.arpigl.listener.EngineListener;

public abstract class AbstractEngineListener implements EngineListener {

    /**
     * address of the native c++ object.
     */
    private long mNativeAddr;

    /**
     * Instantiates a new engine native callbacks.
     */
    public AbstractEngineListener() {
        mNativeAddr = newEngineNativeCallbacks();
    }

    /**
     * Gets the native c++ address.
     *
     * @return the native address
     */
    long getNativeAddr() {
        return mNativeAddr;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        freeEngineNativeCallbacks(mNativeAddr);
    }

    /* ***
     * NATIVE METHODS
     */
    private native long newEngineNativeCallbacks();

    private native void freeEngineNativeCallbacks(long nativeAddr);

}
