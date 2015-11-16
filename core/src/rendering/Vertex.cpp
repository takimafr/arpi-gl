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



#include "rendering/Vertex.hpp"

#include <sstream>


namespace dma {

    Vertex::Vertex() :
                        mPosition(glm::vec3(0.0f)),
                        mFlatNormal(glm::vec3(0.0f)),
                        mSmoothNormal(glm::vec3(0.0f)),
                        mUv(glm::vec2(0.0f))
    {}

    Vertex::~Vertex() {
    }

    const std::string Vertex::toString() const {
        std::ostringstream oss;
        oss << "mPosition=(" << mPosition.x << "," << mPosition.y << "," << mPosition.z << ")"
                << " mFlatNormal=(" << mFlatNormal.x << "," << mFlatNormal.y << "," << mFlatNormal.z << ")"
                << " mSmoothNormal=(" << mSmoothNormal.x << "," << mSmoothNormal.y << "," << mSmoothNormal.z << ")"
                << " mUv=(" << mUv.x << "," << mUv.y << ")" ;
        return oss.str();
    }
}

