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

/*
 * 
 */
package mobi.designmyapp.arpigl.model;

import android.graphics.Color;

import mobi.designmyapp.arpigl.engine.EngineController;

/**
 * Note that a poi is considered equal in regard of its Id.
 *
 * @author Nicolas THIERION
 */
public final class Poi {

    public static final Shape DEFAULT_SHAPE = Shape.Default.BALLOON;
    private static final String UNNAMED_POI_PREFIX = "unnamed";
    private static int gUnamedPoiId = 1;

    /* ***
     * ATTRIBUTES
     */
    String mPoiId;
    String mShapeId;
    String mIcon;
    Integer mColor;
    double mLatitude;
    double mLongitude;
    double mAltitude;

    /* ***
     * CONSTRUCTORS
     */

    /**
     * Create a Poi given its shape, icon & color. The POI will be given a
     * auto-generated id.
     *
     * @param shape the shape sid of this Poi.
     * @param color the color of this poi.
     */
    public Poi(String shape, int color) {
        this(null, shape, null, color);
    }

    /**
     * Create a Poi given its shape, icon & color. The POI will be given a
     * auto-generated id.
     *
     * @param shape the shape sid of this Poi.
     * @param color the color of this poi.
     */
    public Poi(Shape shape, int color) {
        this(shape.getSid(), color);
    }

    /**
     * Create a Poi given its shape, icon & color. The POI will be given a
     * auto-generated id.
     *
     * @param shape the shape sid of this Poi.
     * @param icon  the icon sid of this poi.
     * @param color the color of this poi.
     */
    public Poi(String shape, String icon, int color) {
        this(null, shape, icon, color);
    }

    /**
     * Create a Poi given its shape, icon & color. The POI will be given a
     * auto-generated id.
     *
     * @param shape the shape sid of this Poi.
     * @param icon  the icon sid of this poi.
     * @param color the color of this poi.
     */
    public Poi(Shape shape, String icon, int color) {
        this(shape.getSid(), icon, color);
    }

    /**
     * Create a Poi given its shape, icon & color. The POI will be given a
     * auto-generated id.
     *
     * @param sid   the sid of this Poi.
     * @param shape the shape sid of this Poi.
     * @param icon  the icon sid of this poi.
     * @param color the color of this poi.
     */
    public Poi(String sid, String shape, String icon, int color) {
        this(sid, shape, icon, color, 0, 0, 0);
    }

    /**
     * Create a Poi given its shape, icon & color. The POI will be given a
     * auto-generated id.
     *
     * @param sid   the sid of this Poi.
     * @param shape the shape of this Poi.
     * @param icon  the icon sid of this poi.
     * @param color the color of this poi.
     */
    public Poi(String sid, Shape shape, String icon, int color) {
        this(sid, shape.getSid(), icon, color);
    }

    public Poi(String sid, Shape shape, String icon, int color, double lat, double lng, double alt) {
        this(sid, shape.getSid(), icon, color, lat, lng, alt);
    }


    public Poi(String sid, String shape, String icon, Integer color,
               double lat, double lng, double alt) {
        if (shape == null) {
            shape = DEFAULT_SHAPE.getSid();
        }
        if (sid != null) {
            mPoiId = sid;
        } else {
            mPoiId = generateSid(shape);
        }

        if (color == null) {
            mColor = Color.LTGRAY;
        } else {
            mColor = color;
        }
        mShapeId = shape;
        mIcon = icon;
        mLatitude = lat;
        mLongitude = lng;
        mAltitude = alt;
    }

    /**
     * Gets the latitude.
     *
     * @return the latitude
     */
    public double getLatitude() {
        return mLatitude;
    }

    /* ***
     * GETTERS
     */

    /**
     * Gets the longitude.
     *
     * @return the longitude
     */
    public double getLongitude() {
        return mLongitude;
    }

    /**
     * Gets the altitude.
     *
     * @return the altitude
     */
    public double getAltitude() {
        return mAltitude;
    }

    /**
     * Gets the mesh id.
     *
     * @return the mesh id
     */
    public String getShapeId() {
        return mShapeId;
    }

    /**
     * Gets the icon id.
     *
     * @return the icon id
     */
    public String getIconId() {
        return mIcon;
    }

    /**
     * Gets the poi SID.
     *
     * @return the poi SID
     */
    public String getId() {
        return mPoiId;
    }

    /**
     * Gets the poi color.
     *
     * @return the poi color
     */
    public int getColor() {
        return mColor;
    }

    public boolean hasIcon() {
        return mIcon != null && !mIcon.trim().isEmpty();
    }

    /**
     * Provide a transactional state for this Poi.
     *
     * @param engine the engine controller.
     * @return the PoiEditor
     */
    public PoiEditor edit(EngineController engine) {
        return new PoiEditor(this, engine);
    }

    private String generateSid(String shape) {
        synchronized (Poi.class) {
            return UNNAMED_POI_PREFIX + "_" + shape + "_" + Integer.toString(gUnamedPoiId++);
        }
    }

    /* ***
     * EQUALS & HASHCODE
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }

        Poi that = (Poi) o;

        return !(mPoiId != null ? !mPoiId.equals(that.mPoiId) : that.mPoiId != null);

    }

    @Override
    public int hashCode() {
        return mPoiId != null ? mPoiId.hashCode() : 0;
    }

    public static final Builder builder() {
        return new Builder();
    }

    /**
     * Builder for the {@link Poi} class.
     */
    public static final class Builder {

        String mPoiId;
        String mShapeId;
        String mIcon;
        Integer mColor;
        double mLatitude;
        double mLongitude;
        double mAltitude;

        /**
         * Instantiates a new builder. Poi will be created with
         * {@link #DEFAULT_SHAPE} shape by default.
         */
        public Builder() {
            mColor = Color.LTGRAY;
            mShapeId = DEFAULT_SHAPE.getSid();
        }

        /**
         * Sets the shape of the poi.
         *
         * @param shapeSid the shape sid
         * @return this builder
         */
        public Builder shape(String shapeSid) {
            mShapeId = shapeSid;
            return this;
        }

        /**
         * Sets the shape of the poi.
         *
         * @param shape the shape
         * @return this builder
         */
        public Builder shape(Shape shape) {
            return shape(shape.getSid());
        }

        /**
         * Sets the latitude of the poi.
         *
         * @param latitude the latitude
         * @return this builder
         */
        public Builder latitude(double latitude) {
            mLatitude = latitude;
            return this;
        }

        /**
         * Sets the longitude of the poi.
         *
         * @param longitude the longitude
         * @return this builder
         */
        public Builder longitude(double longitude) {
            mLongitude = longitude;
            return this;
        }

        /**
         * Sets the altitude of the poi.
         *
         * @param altitude the altitude
         * @return this builder
         */
        public Builder altitude(double altitude) {
            mAltitude = altitude;
            return this;
        }

        /**
         * Sets the position of the poi.
         *
         * @param latitude  the latitude
         * @param longitude the longitude
         * @param altitude  the altitude
         * @return this builder
         */
        public Builder position(double latitude, double longitude, double altitude) {
            altitude(altitude);
            latitude(latitude);
            longitude(longitude);
            return this;
        }

        /**
         * Sets the icon of the poi.
         *
         * @param icon the icon id
         * @return this builder
         */
        public Builder icon(String icon) {
            mIcon = icon;
            return this;
        }

        /**
         * Sets the color of the poi.
         *
         * @param color the color
         * @return this builder
         */
        public Builder color(int color) {
            mColor = color;
            return this;
        }

        /**
         * Sets the id of the poi.
         *
         * @param id the id
         * @return the builder
         */
        public Builder id(String id) {
            mPoiId = id;
            return this;
        }

        /**
         * Clone builder state from existing Poi.
         *
         * @param poi the poi to deep copy.
         * @return the builders
         */
        public Builder fromPoi(Poi poi) {
            this.mShapeId = poi.mShapeId;
            this.mPoiId = poi.mPoiId;
            this.mIcon = poi.mIcon;
            this.mColor = poi.mColor;
            this.mLatitude = poi.mLatitude;
            this.mLongitude = poi.mLongitude;
            this.mAltitude = poi.mAltitude;
            return this;
        }

        /**
         * Builds the poi.
         *
         * @return the poi
         */
        public Poi build() {
            return new Poi(mPoiId, mShapeId, mIcon , mColor, mLatitude, mLongitude, mAltitude);
        }
    }


}
