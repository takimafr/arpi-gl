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



#include "rendering/Plane.hpp"


namespace dma {


    Plane::Plane() { }


    Plane::Plane(const glm::vec3& normal, const glm::vec3& point) :
        mNormal(normal),
        mPoint(point)
    {}



    Plane::~Plane() {

    }



    float Plane::distance(const glm::vec3& p) {
        //float D = glm::dot<float>(-mNormal, mPoint);

        //float d = glm::dot<float>(p - mPoint, mNormal);
        //float d = glm::dot<float>(mNormal, p) + D;
        float d = glm::dot<float>(p - mPoint, mNormal);
        return d;
    }
}