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


#ifndef _DMA_VERTEXBUFFER_H_
#define _DMA_VERTEXBUFFER_H_

#include <utils/GLES2Logger.hpp>

#include <string>

#include "common/Types.hpp"
#include "rendering/VertexElement.hpp"


namespace dma {

    /**
     * Wrapper of OpenGL vertex buffer object.
     */
    class VertexBuffer {

    public:
        VertexBuffer();
        VertexBuffer(U32 vertexSize, U32 sizeInByte);
        virtual ~VertexBuffer();

        void generateBuffer(U32 vertexSize, U32 sizeInByte);

        /**
         * Returns the OpenGL handle.
         */
        inline GLuint getHandle() const {
            return mHandle;
        }
        /**
         * Returns the size in byte of the VBO.
         */
        inline U32 getSizeInByte() const {
            return mSizeInByte;
        }

        /**
         * Writes sub data to the VBO
         */
        void writeData(U32 offset, U32 length, const void *source);

        /**
         * Delete the OpenGL buffer.
         */
        void wipe();

        const std::string toString() const;

    private:
        GLuint mHandle;
        U32 mVertexSize;
        U32 mSizeInByte;
    };
}



#endif //_DMA_VERTEXBUFFER_H_
