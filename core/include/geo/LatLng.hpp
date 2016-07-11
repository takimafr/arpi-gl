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


#ifndef ARPIGL_LATLNG_HPP
#define ARPIGL_LATLNG_HPP

namespace dma {

    class LatLng {

    public:
        LatLng(double lat, double lng) :
                lat(lat), lng(lng) {}

        LatLng() :
                LatLng(0.0, 0.0) {}

        inline bool operator==(const LatLng& other) {
            return lat == other.lat && lng == other.lng;
        }

        double lat;
        double lng;
    };
}

#endif //ARPIGL_LATLNG_HPP
