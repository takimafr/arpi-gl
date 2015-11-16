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


#ifndef _DMA_QUADFACTORY_HPP_
#define _DMA_QUADFACTORY_HPP_

#include "rendering/VertexBuffer.hpp"
#include "rendering/IndexBuffer.hpp"
#include "resource/Quad.hpp"

#include <memory>

namespace dma {

    class QuadFactory {

        friend class ResourceManager;

    public:
        std::shared_ptr<Quad> createQuad(F32 width, F32 height);

    private:
        QuadFactory();
        virtual ~QuadFactory();

        QuadFactory(const QuadFactory&) = delete;
        void operator=(const QuadFactory&) = delete;

        void init();
        void unload();

        /*
         * Clean all GPU resources
         */
        void wipe();

        void refresh();

        /**
         * Creates a new quad width x height
         * @note You must not delete the quad but instead call the deleteQuad method
         * @param width the width of the quad
         * @param height the height of the quad
         */

        /**
         * Deletes the quad
         * @param quad should be an object created by a call to createQuad
         */
        void deleteQuad(const Quad& quad);

    private:
        //FIELDS
        U32 mVertexSize;
        U32 mVertexCount;
        std::shared_ptr<VertexBuffer> mVertexBuffer;
        std::shared_ptr<IndexBuffer> mIndexBuffer;
    };
}

#endif //_DMA_QUADFACTORY_HPP_
