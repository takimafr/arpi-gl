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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;

public class CachedLinkedHashSet<E> extends LinkedHashSet<E> {

    private E mLast;
    private int mSize;

    public CachedLinkedHashSet(int size) {
        super(size);
        this.mSize = size;
    }

    public final E push(E e) {
        mLast = e;
        if (!super.add(e)) {
            refresh(e);
            return null;
        }
        if (size() == mSize + 1) {
            return shrink(mSize).get(0);
        }
        return null;
    }

    public final E getLast() {
        return mLast;
    }

    public final void refresh(E object) {
        super.remove(object);
        super.add(object);
    }

    public final List<E> shrink(int n) {
        List<E> deleted = new ArrayList<>();

        int overflow = size() - n;

        Iterator<E> it = this.iterator();
        while (it.hasNext()) {
            if (overflow == 0) {
                break;
            }
            deleted.add(it.next());
            it.remove();
            --overflow;
        }
        return deleted;
    }

}
