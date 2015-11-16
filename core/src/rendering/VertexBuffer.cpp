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



#include <sstream>

#include "rendering/VertexBuffer.hpp"

constexpr auto TAG = "VertexBuffer";

namespace dma {

    VertexBuffer::VertexBuffer() :
                        mHandle(0),
                        mVertexSize(0),
                        mSizeInByte(0)
    {}

    VertexBuffer::VertexBuffer(U32 vertexSize, U32 sizeInByte) : VertexBuffer() {
        generateBuffer(vertexSize, sizeInByte);
    }

    VertexBuffer::~VertexBuffer() {
        //wipe();
    }


    void VertexBuffer::generateBuffer(U32 vertexSize, U32 sizeInByte) {
        mVertexSize = vertexSize;
        mSizeInByte = sizeInByte;
        //Log::debug(TAG, "generating GL Buffer of size %d bytes...", mSizeInByte);
        glGenBuffers(1, &mHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mHandle);
        glBufferData(GL_ARRAY_BUFFER, mSizeInByte, NULL, GL_STATIC_DRAW);

    }


    void VertexBuffer::writeData(U32 offset, U32 length, const void *source) {
        glBindBuffer(GL_ARRAY_BUFFER, mHandle);
        //Log::info("offset=%d length=%d source=%p", offset, length, source);
        glBufferSubData(GL_ARRAY_BUFFER, offset, length, source);
    }


    const std::string VertexBuffer::toString() const {
        std::ostringstream oss;
        oss << "mHandle=" << mHandle << " mVertexSize=" << mVertexSize << " mSizeInByte=" << mSizeInByte;
        return oss.str();
    }


    void VertexBuffer::wipe() {
        if(glIsBuffer(mHandle) == GL_TRUE) {
            glDeleteBuffers(1, &mHandle);
        }
        mHandle = 0;
    }
}
