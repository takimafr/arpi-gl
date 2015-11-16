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


#ifndef _DMA_VERTEXELEMENT_HPP_
#define _DMA_VERTEXELEMENT_HPP_


#include <cassert>
#include <string>
#include <sstream>
#include <GLES2/gl2.h>



#include "utils/GLES2Logger.hpp"
#include "common/Types.hpp"
#include "resource/ShaderProgram.hpp"


namespace dma {

    /**
     * Contains information about a vertex element:
     * - its semantic
     * - its count and type, for example: 3 GL_FLOAT
     * - its offset in the vbo, for example:
     *        if normal is after position and position has
     *        3 GL_FLOAT, offset of normal will be 3 * sizeof(GLfloat)
     */
    class VertexElement {

    public:

        enum Semantic {
            POSITION = 0,
            UV = 1,
            FLAT_NORMAL = 2,
            SMOOTH_NORMAL = 3,
            SIZE = 5,
            NONE = 6,
        };

        VertexElement() :
            mSemantic(NONE),
            mCount(0),
            mType(-1),
            mSizeInByte(0),
            mOffset(0)
        {}
        VertexElement(Semantic semantic, U32 count, GLenum type, U32 offset) :
            mSemantic(semantic),
            mCount(count),
            mType(type),
            mOffset(offset)
        {
            switch (type) {
            case GL_FLOAT:
                mSizeInByte = (U32)(count * sizeof(GLfloat));
                break;
            default:
                assert(false);
            }
        }

        virtual ~VertexElement(){}

        inline const Semantic& getSemantic() const {
            return mSemantic;
        }
        inline GLenum getType() const {
            return mType;
        }
        inline U32 getCount() const {
            return mCount;
        }
        inline U32 getOffset() const {
            return mOffset;
        }
        inline U32 getSizeInByte() const {
            return mSizeInByte;
        }

        const std::string toString() const {
            std::ostringstream oss;
            std::string sem;
            switch (mSemantic) {
            case Semantic::POSITION:
                sem = "POSITION";
                break;
            case Semantic::FLAT_NORMAL:
                sem = "FLAT_NORMAL";
                break;
            case Semantic::SMOOTH_NORMAL:
                sem = "SMOOTH_NORMAL";
                break;
            case Semantic::UV:
                sem = "UV";
                break;
            default:
                assert(false);
            }
            oss << "mSemantic=" << sem << " mCount=" << mCount << " mType=" << mType << " mSizeInByte="
                    << mSizeInByte << " mOffset=" << mOffset;
            return oss.str();
        }

    private:
        VertexElement::Semantic mSemantic;
        U32 mCount;
        GLenum mType;
        U32 mSizeInByte;
        U32 mOffset;
    };
}

#endif //_DMA_VERTEXELEMENT_HPP_
