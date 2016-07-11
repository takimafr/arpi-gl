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



#include <glm/gtc/constants.hpp>
#include "utils/GeoUtils.hpp"
#include "glm/glm.hpp"

namespace dma {

        double GeoUtils::slc(LatLng coords1, LatLng coords2) {

            if (coords1 == coords2) {
                return 0.0;
            }

            double r = 6371000.0;
            double dlambda = glm::radians(coords1.lng - coords2.lng);
            double phi1 = glm::radians(coords1.lat);
            double phi2 = glm::radians(coords2.lat);

            double sinPhi1 = sin(phi1);
            double sinPhi2 = sin(phi2);
            double cosPhi1 = cos(phi1);
            double cosPhi2 = cos(phi2);
            double cosLambda = cos(dlambda);

            double val = sinPhi1 * sinPhi2 + cosPhi1 * cosPhi2 * cosLambda;
            if (val > 1.0) {
                val = 1.0;
            } else if (val < -1.0) {
                val = -1.0;
            }
            return acos(val) * r;
        }



        double GeoUtils::bearing(LatLng coords1, LatLng coords2) {
            double dlambda = glm::radians(coords1.lng - coords2.lng);
            double phi1 = glm::radians(coords2.lat);
            double phi2 = glm::radians(coords1.lat);

            double y = sin(dlambda) * cos(phi2);
            double x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(dlambda);
            double bearing = glm::degrees(atan2(y, x)); // [-180, 180]
            return glm::mod(bearing + 360.0, 360.0);
        }

        glm::vec3 GeoUtils::vector(LatLngAlt coords1, LatLngAlt coords2) {
            double bearing = GeoUtils::bearing(coords2, coords1);
            double distance = GeoUtils::slc(coords2, coords1);
            glm::vec3 res;
            double theta = (glm::half_pi<double>() - glm::radians(bearing));
            res.x = (float) (distance * cos(theta));
            res.y = coords2.alt - coords1.alt;
            res.z = (float) (-distance * sin(theta));
            return res;
        }

}
