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


#ifndef _DMA_MESH_HPP_
#define _DMA_MESH_HPP_

#include "rendering/VertexBuffer.hpp"
#include "rendering/IndexBuffer.hpp"
#include "rendering/VertexElement.hpp"
#include "utils/VertexIndices.hpp"
#include "resource/GpuResource.hpp"

#include <vector>
#include <set>
#include <memory>
#include <rendering/BoundingSphere.hpp>

namespace dma {
    class Mesh : public GpuResource {

    public:
        Mesh();
        virtual ~Mesh();

    protected:
        friend class MeshManager;

        Mesh(const Mesh&) = delete;
        void operator=(const Mesh&) = delete;

    public:
        inline const std::string& getSID() const {
            return mSID;
        }

        inline bool hasFlatNormals() const {
            return hasVertexElement(VertexElement::Semantic::FLAT_NORMAL);
        }

        inline U32 getVertexSize() const {
            return mVertexSize;
        }

        inline U32 getVertexCount() const {
            return mVertexCount;
        }

        inline const VertexBuffer& getVertexBuffer() const {
            return *mVertexBuffer;
        }

        inline const IndexBuffer& getIndexBuffer() const {
            return *mIndexBuffer;
        }

        inline U32 getVertexSemFlags() const {
            return mVertexSemFlags;
        }

        inline bool hasVertexElement(VertexElement::Semantic sem) const {
            return (mVertexSemFlags & (1L << sem)) != 0;
        }

        inline const VertexElement& getVertexElement(VertexElement::Semantic semantic) const {
            return mVertexElements[semantic];
        }

        inline const BoundingSphere& getBoundingSphere() const { return mBoundingSphere; }

        /**
         * Clear OpenGL resources
         */
        void wipe() override;

        void clearCache() override;


    protected:
        //METHODS
        void addVertexElement(const VertexElement& vertexElement);

        inline const bool hasCache() {
            return !positions.empty();
        }


        //FIELDS
        std::string mSID;
        VertexElement mVertexElements[VertexElement::Semantic::SIZE];
        U32 mVertexSemFlags;
        U32 mVertexSize;
        U32 mVertexCount;
        std::shared_ptr<VertexBuffer> mVertexBuffer;
        std::shared_ptr<IndexBuffer> mIndexBuffer;
        BoundingSphere mBoundingSphere;

        //Cache variables
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> flatNormals;
        std::vector<glm::vec3> smoothNormals;
        std::vector<VertexIndices> vertexIndices;
    };
}

#endif //_DMA_MESH_HPP_
