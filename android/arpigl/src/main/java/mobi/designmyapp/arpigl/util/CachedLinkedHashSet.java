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

import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.NoSuchElementException;
import java.util.Queue;

public class CachedLinkedHashSet<E> extends LinkedHashSet<E> implements Queue<E> {

    private E mLast;
    private int mCapacity;

    public CachedLinkedHashSet(int size) {
        super(size);
        this.mCapacity = size;
    }

    private boolean moveToTail(E object) {
        super.remove(object);
        return super.add(object);
    }

    @Override
    public boolean offer(E e) {
        mLast = e;
        boolean result = false;
        // If element exists
        if (!super.add(e)) {
            result = moveToTail(e);
        }
        if (size() > mCapacity) {
            throw new IndexOutOfBoundsException();
        }
        return result;
    }

    @Override
    public E remove() {
        E element = poll();
        if (element == null) {
            throw new NoSuchElementException();
        }
        return element;
    }

    @Override
    public E poll() {
        Iterator<E> it = super.iterator();
        if (it.hasNext()) {
            E element = it.next();
            it.remove();
            return element;
        }
        return null;
    }

    @Override
    public E element() {
        E element = poll();
        if (element == null) {
            throw new NoSuchElementException();
        }
        return element;
    }

    @Override
    public E peek() {
        Iterator<E> it = super.iterator();
        if (it.hasNext()) {
            E element = it.next();
            return element;
        }
        return null;
    }

    public int getCapacity() {
        return mCapacity;
    }

    public boolean isFull() {
        return size() >= mCapacity;
    }
}
