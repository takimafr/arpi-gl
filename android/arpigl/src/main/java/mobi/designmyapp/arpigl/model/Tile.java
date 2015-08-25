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

package mobi.designmyapp.arpigl.model;

import java.util.Arrays;

public final class Tile {

    private byte[] mImageData;
    private Tile.Id mTileId;

    public Tile(byte[] data, int x, int y, int z) {
        mImageData = data;
        mTileId = new Tile.Id(x, y, z);
    }

    public Tile(byte[] data, Id tid) {
        this(data, tid.x, tid.y, tid.z);
    }

    public byte[] getData() {
        return mImageData;
    }

    public Tile.Id getId() {
        return mTileId;
    }

    @Override
    public String toString() {
        return "Tile{" + "mImageData=" + Arrays.toString(mImageData) + ", mTileId=" + mTileId + '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Tile tile = (Tile) o;

        return !(mTileId != null ? !mTileId.equals(tile.mTileId) : tile.mTileId != null);

    }

    @Override
    public int hashCode() {
        return mTileId != null ? mTileId.hashCode() : 0;
    }

    public static final class Id {

        public final int x;
        public final int y;
        public final int z;

        public Id(int x, int y, int z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) {
                return true;
            }
            if (o == null || getClass() != o.getClass()) {
                return false;
            }

            Id id = (Id) o;

            if (x != id.x) {
                return false;
            }
            if (y != id.y) {
                return false;
            }
            return z == id.z;

        }

        @Override
        public int hashCode() {
            int result = x;
            final int prime = 31;
            result = prime * result + y;
            result = prime * result + z;
            return result;
        }

        @Override
        public String toString() {
            return z + "/" + x + "/" + y;
        }
    }
}
