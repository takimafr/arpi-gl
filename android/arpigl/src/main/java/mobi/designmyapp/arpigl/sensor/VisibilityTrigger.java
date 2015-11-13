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

package mobi.designmyapp.arpigl.sensor;

import android.app.Fragment;

/**
 * A {@code VisibilityTrigger} handle a {@link Fragment} visibility, according
 * to some defined rules.
 *
 * @author Nicolas THIERION
 */
public abstract class VisibilityTrigger {

    /* ***
     * ATTRIBUTES
     */

    /**
     * the fragment to show or hide.
     */
    private Fragment mFragment;

    /**
     * the listener on display changes
     */
    private OnDisplayChangeListener onDisplayChangeListener;

    /**
     * True if the fragment is visible
     */
    private boolean isFragmentVisible;

    /* ***
     * CONSTRUCTOR
     */

    /**
     * Create a new {@code VisibilityTrigger} to manage the visibility of given
     * fragment.
     *
     * @param fragment the fragment to handle visibility.
     */
    public VisibilityTrigger(Fragment fragment) {
        mFragment = fragment;
    }

    /**
     * @return true if this {@code VisibilityTrigger} is enabled.
     */
    public abstract boolean isEnabled();

    /**
     * Enables or disables this {@code VisibilityTrigger}.
     *
     * @param enabled if this trigger should be enabled.
     */
    public abstract void setEnabled(boolean enabled);

    /**
     * @return true if the managed fragment is visible.
     */
    public final boolean isShown() {
        return isFragmentVisible;
    }

    /**
     * Remove the  {@code OnDisplayChangeListener}
     */
    public void removeOnDisplayChangeListener() {
        onDisplayChangeListener = null;
    }

    /**
     * Set the onDisplayChangeListener
     * @param onDisplayChangeListener the {@code OnDisplayChangeListener} to set
     */
    public void setOnDisplayChangeListener(OnDisplayChangeListener onDisplayChangeListener) {
        this.onDisplayChangeListener = onDisplayChangeListener;
    }

    /**
     * Sets the visibility of the managed fragment.
     *
     * @param visible if the fragment should be visible.
     */
    public final synchronized void show(boolean visible) {
        if (visible && !isFragmentVisible) {
            if (onDisplayChangeListener != null) {
                onDisplayChangeListener.beforeDisplayChange(true);
            }
            isFragmentVisible = true;
            mFragment.getFragmentManager().beginTransaction().show(mFragment).commit();
            if (onDisplayChangeListener != null) {
                onDisplayChangeListener.afterDisplayChange(true);
            }
        } else if (!visible && isFragmentVisible) {
            if (onDisplayChangeListener != null) {
                onDisplayChangeListener.beforeDisplayChange(false);
            }
            isFragmentVisible = false;
            mFragment.getFragmentManager().beginTransaction().hide(mFragment).commit();
            if (onDisplayChangeListener != null) {
                onDisplayChangeListener.afterDisplayChange(false);
            }
        }
    }

    public interface OnDisplayChangeListener {
        /**
         * Called before the fragment visibility change
         * @param visible if the fragment will be visible.
         */
        void beforeDisplayChange(boolean visible);

        /**
         * Called after the fragment visibility change
         * @param visible if the fragment is visible.
         */
        void afterDisplayChange(boolean visible);
    }
}

