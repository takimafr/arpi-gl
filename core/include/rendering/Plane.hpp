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


#ifndef ARPIGL_PLANE_HPP
#define ARPIGL_PLANE_HPP

#include "glm/glm.hpp"

namespace dma {
    class Plane {

        glm::vec3 mNormal;
        glm::vec3 mPoint;

    public:
        Plane();
        Plane(const glm::vec3& normal, const glm::vec3& point);
        virtual ~Plane();

        inline const glm::vec3 &getNormal() const {
            return mNormal;
        }

        inline void setNormal(const glm::vec3 &mNormal) {
            Plane::mNormal = mNormal;
        }

        inline const glm::vec3 &getPoint() const {
            return mPoint;
        }

        inline void setPoint(const glm::vec3 &mPoint) {
            Plane::mPoint = mPoint;
        }

        float distance(const glm::vec3& p);
    };
}


#endif //ARPIGL_PLANE_HPP
