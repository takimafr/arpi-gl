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



#include "glm/gtc/matrix_transform.hpp"
#include "resource/Quad.hpp"

constexpr auto TAG = "Quad";

namespace dma {


    //--------------------------------------------------
    Quad::Quad(float width, float height,
            const U32 vertexSize,
            const U32 vertexCount,
            std::shared_ptr<VertexBuffer> vertexBuffer,
            std::shared_ptr<IndexBuffer> indexBuffer) :
                        Mesh(),
                        mWidth(width),
                        mHeight(height)
    {
        Mesh::mVertexSize = vertexSize;
        Mesh::mVertexCount = vertexCount;
        Mesh::mVertexBuffer = vertexBuffer;
        Mesh::mIndexBuffer = indexBuffer;

        //            Log::debug(TAG, "VertexBuffer: %s", mVertexBuffer.toString().c_str());
        //            Log::debug(TAG, "IndexBuffer: %s", mIndexBuffer.toString().c_str());

        U32 offset = 0;
        VertexElement positionElement(VertexElement::Semantic::POSITION, 3, GL_FLOAT, offset);
        offset += positionElement.getSizeInByte();
        addVertexElement(positionElement);

        VertexElement uvElement(VertexElement::Semantic::UV, 2, GL_FLOAT, offset);
        offset += uvElement.getSizeInByte();
        addVertexElement(uvElement);

        VertexElement flatNormalElement(VertexElement::Semantic::FLAT_NORMAL, 3, GL_FLOAT, offset);
        offset += flatNormalElement.getSizeInByte();
        addVertexElement(flatNormalElement);

        VertexElement smoothNormalElement(VertexElement::Semantic::SMOOTH_NORMAL, 3, GL_FLOAT, offset);
        addVertexElement(smoothNormalElement);

        mScale = glm::vec3(width / 2.0f, height / 2.0f, 1.0f);
        mBoundingSphere = BoundingSphere(0.0f, 0.0f, 0.0f, glm::sqrt(width * width + height * height) / 2.0f);
    }


    //--------------------------------------------------
    Quad::~Quad() {

    }


    //--------------------------------------------------
    void Quad::setSize(float width, float height) {
        mWidth = width;
        mHeight = height;
        mScale = glm::vec3(width / 2.0f, height / 2.0f, 1.0f);
        mBoundingSphere = BoundingSphere(0.0f, 0.0f, 0.0f, glm::sqrt(width * width + height * height) / 2.0f);
    }
}
