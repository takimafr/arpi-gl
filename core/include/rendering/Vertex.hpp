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


#ifndef _DMA_VERTEX_HPP_
#define _DMA_VERTEX_HPP_

#include "glm/glm.hpp"

#include <string>

namespace dma {

    class Vertex {
    public:
        Vertex();
        virtual ~Vertex();


        inline const glm::vec3& getPosition() const {
            return mPosition;
        }
        inline void setPosition(const glm::vec3& position) {
            mPosition = position;
        }

        inline void setUv(const glm::vec2& uv) {
            mUv = uv;
        }
        inline glm::vec2& getUv() {
            return mUv;
        }
        inline const glm::vec3& getFlatNormal() const {
            return mFlatNormal;
        }
        inline void setFlatNormal(const glm::vec3& normal) {
            mFlatNormal = normal;
        }
        inline const glm::vec3& getSmoothNormal() const {
            return mSmoothNormal;
        }
        inline void setSmoothNormal(const glm::vec3& normal) {
            mSmoothNormal = normal;
        }

        const std::string toString() const;


    private:
        glm::vec3 mPosition;
        glm::vec3 mFlatNormal;
        glm::vec3 mSmoothNormal;
        glm::vec2 mUv;
    };

}

#endif //_DMA_VERTEX_HPP_
