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

package mobi.designmyapp.arpigl.util;

import android.util.Log;

import java.lang.ref.WeakReference;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Collections;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by Loïc Ortola on 01/09/2015.
 */
public class InternalBus {

    private static final String DEFAULT_EVENT_METHOD = "onEvent";
    private static final String TAG = InternalBus.class.getSimpleName();

    private static InternalBus mInternalBus;

    private Set<WeakReference> mObservers;


    public static InternalBus getInstance() {
        if (mInternalBus == null) {
            mInternalBus = new InternalBus();
        }
        return mInternalBus;

    }

    private InternalBus() {
        mObservers = Collections.newSetFromMap(new ConcurrentHashMap());
    }

    public void register(Object target) {
        Iterator<WeakReference> it = mObservers.iterator();
        while (it.hasNext()) {
            WeakReference ref = it.next();
            Object observer = ref.get();
            if (observer == null) {
                mObservers.remove(ref);
                continue;
            }
            if (target.equals(observer)) {
                return;
            }
        }
        mObservers.add(new WeakReference<>(target));
    }

    public void unregister(Object target) {
        Iterator<WeakReference> it = mObservers.iterator();
        while (it.hasNext()) {
            WeakReference ref = it.next();
            Object observer = ref.get();
            if (observer == null) {
                mObservers.remove(ref);
                continue;
            }
            if (target.equals(observer)) {
                mObservers.remove(ref);
            }
        }
    }

    public void post(Object event) {
        Iterator<WeakReference> it = mObservers.iterator();
        while (it.hasNext()) {
            WeakReference ref = it.next();
            Object observer = ref.get();
            if (observer == null) {
                mObservers.remove(ref);
                continue;
            }
            postEvent(observer, event);
        }
    }

    public boolean hasObservers(Class eventClass) {
        Iterator<WeakReference> it = mObservers.iterator();
        while (it.hasNext()) {
            WeakReference ref = it.next();
            Object observer = ref.get();
            if (observer == null) {
                mObservers.remove(ref);
                continue;
            }
            if (hasObservers(observer, eventClass)) {
                return true;
            }
        }
        return false;
    }

    private void postEvent(Object observer, Object event) {
        Class clazz = observer.getClass();
        for (Method t : clazz.getDeclaredMethods()) {
            if (t.getName().equals(DEFAULT_EVENT_METHOD)) {
                Class<?>[] parameterTypes = t.getParameterTypes();
                if (parameterTypes.length == 1 && parameterTypes[0].equals(event.getClass())) {
                    try {
                        t.invoke(observer, event);
                    } catch (IllegalAccessException e) {
                        Log.e(TAG, "Cannot invoke onEvent method! Illegal Access");
                    } catch (InvocationTargetException e) {
                        Log.e(TAG, "Cannot invoke onEvent method! Invocation Target Exception occurred");
                    }
                }
            }
        }
    }

    private boolean hasObservers(Object observer, Class eventClass) {
        Class clazz = observer.getClass();
        for (Method t : clazz.getDeclaredMethods()) {
            if (t.getName().equals(DEFAULT_EVENT_METHOD)) {
                Class<?>[] parameterTypes = t.getParameterTypes();
                if (parameterTypes.length == 1 && parameterTypes[0].equals(eventClass)) {
                    return true;
                }
            }
        }
        return false;
    }
}
