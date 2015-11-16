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


#ifndef _INDEXBUFFER_H_
#define _INDEXBUFFER_H_

#include "common/Types.hpp"

#include <string>


namespace dma {

    /**
     * Wrapper of OpenGL index buffer object.
     */
    class IndexBuffer {

    public:
        IndexBuffer();
        //IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer(U32 elementCount);
        virtual ~IndexBuffer();

        void generateBuffer(U32 elementCount);

        /**
         * Returns the OpenGL handle
         */
        inline GLuint getHandle() const {
            return mHandle;
        }
        /**
         * Returns the size in byte of the IBO
         */
        inline U64 getSizeInByte() const {
            return mSizeInByte;
        }

        /**
         * Returns the number of elements in that IBO
         */
        inline U32 getElementCount() const {
            return mElementCount;
        }

        /**
         * Writes mSizeInByte bytes of data to the IBO GPU side
         * data must be mSizeInByte long
         */
        void writeData(const void* data);

        /**
         * Delete the OpenGL buffer.
         */
        void wipe();

        const std::string toString() const;


    private:
    GLuint mHandle;
        U64 mSizeInByte;
        U32 mElementCount;
    };
}



#endif //_INDEXBUFFER_H_
