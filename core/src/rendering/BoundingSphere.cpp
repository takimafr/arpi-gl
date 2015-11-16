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



#include "rendering/BoundingSphere.hpp"

namespace dma {

    //-------------------------------------------------------------------------------
    BoundingSphere::BoundingSphere() :
        BoundingSphere(0.0f, 0.0f, 0.0f, 0.0f)
    {}


    //-------------------------------------------------------------------------------
    BoundingSphere::BoundingSphere(float x, float y, float z, float r) :
        mCenter(x, y, z),
        mRadius(r)
    {}


    //-------------------------------------------------------------------------------
    BoundingSphere::BoundingSphere(const glm::vec3 &center, float radius) :
        BoundingSphere(center.x, center.y, center.z, radius)
    {}


    //-------------------------------------------------------------------------------
    BoundingSphere::~BoundingSphere() {

    }
}