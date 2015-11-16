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


#ifndef _DMA_QUAD_HPP_
#define _DMA_QUAD_HPP_

#include "glm/glm.hpp"
#include "resource/Mesh.hpp"

namespace dma {

    class Quad : public Mesh {

    public:
        Quad(float width, float height,
                const U32 vertexSize,
                const U32 vertexCount,
                std::shared_ptr<VertexBuffer> vertexBuffer,
                std::shared_ptr<IndexBuffer> indexBuffer);
        virtual ~Quad();

        Quad(const Quad&) = delete;
        void operator=(const Quad&) = delete;

        inline const glm::vec3& getScale() const {return mScale;}

        void setSize(float width, float height);

        inline float getWidth() const { return mWidth; }

        inline float getHeight() const { return mHeight; }

    private:
        float mWidth;
        float mHeight;
        glm::vec3 mScale;
    };
}

#endif //_DMA_QUAD_HPP_
