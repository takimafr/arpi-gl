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



#include <stddef.h>
#include <sstream>

#include <GLES2/gl2.h>

#include "rendering/IndexBuffer.hpp"
#include "utils/Log.hpp"
#include "utils/GLES2Logger.hpp"

constexpr auto TAG = "IndexBuffer";

namespace dma {

    IndexBuffer::IndexBuffer() :
            mHandle(0),
            mSizeInByte(0),
            mElementCount(0)
    {}

    IndexBuffer::IndexBuffer(U32 elementCount) : IndexBuffer() {
        generateBuffer(elementCount);
    }

    IndexBuffer::~IndexBuffer() {
    }


    void IndexBuffer::generateBuffer(U32 elementCount) {
        mSizeInByte = elementCount * sizeof(GLushort);
        mElementCount = elementCount;
        glGenBuffers(1, &mHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSizeInByte, NULL, GL_STATIC_DRAW);

    }


    void IndexBuffer::writeData(const void* source) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mSizeInByte, source);
//        int size;
//        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
//        Log::debug(TAG, "size=%d", size);
    }

    void IndexBuffer::wipe() {
        if (glIsBuffer(mHandle) == GL_TRUE) {
            glDeleteBuffers(1, &mHandle);
        }
        mHandle = 0;
    }

    const std::string IndexBuffer::toString() const {
        std::ostringstream oss;
        oss << "mHandle=" << mHandle << " mSizeInByte=" << mSizeInByte << " mElementCount=" << mElementCount;
        return oss.str();
    }


}
