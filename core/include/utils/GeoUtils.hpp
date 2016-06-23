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


#ifndef _DMA_GEOUTILS_HPP_
#define _DMA_GEOUTILS_HPP_

#include <cmath>
#include <engine/geo/LatLng.hpp>
#include <glm/vec3.hpp>
#include <engine/geo/LatLngAlt.hpp>

namespace dma {
    namespace geo {

        class GeoUtils {

        public:


            static inline int lng2tilex(double lon, int z) {
                return (int)(floor((lon + 180.0) / 360.0 * (double)(1 << z)));
            }



            static inline int lat2tiley(double lat, int z) {
                return (int)(floor((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * (double)(1 << z)));
            }



            static inline double tilex2long(int x, int z) {
                return x / (double)(1 << z) * 360.0 - 180;
            }



            static inline double tiley2lat(int y, int z) {
                double n = M_PI - 2.0 * M_PI * y / (double)(1 << z);
                return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
            }


            /**
            * Spherical Law of Cosines
            * Returns the distance in meter between coords1 and coords2
            */
            static double slc(LatLng coords1, LatLng coords2);


            /**
             * Computes the bearing between 2 coordinates from coords2 to coords1.
             * @param coords1 the arrival point
             * @param coords2 the starting point
             * @return a clockwise angle from north in degrees
             */
            static double bearing(LatLng coords1, LatLng coords2);


            static glm::vec3 vector(LatLngAlt coords1, LatLngAlt coords2);

        };
    }
}

#endif //_DMA_GEOUTILS_HPP_
