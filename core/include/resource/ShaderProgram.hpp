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


#ifndef _DMA_SHADER_PROGRAM_HPP_
#define _DMA_SHADER_PROGRAM_HPP_

#include <utils/GLES2Logger.hpp>
#include "common/Types.hpp"
#include "utils/ExceptionType.hpp"
#include <string>

namespace dma {
    class ShaderProgram {

        friend class ShaderManager;
        static constexpr char TAG[] = "ShaderProgram";

    public:
        enum AttribSem
        {
            POS = 0,
            NORMAL = 1,
            UV = 2,
            AS_size = 3
        };
        enum UniformSem
        {
            MV = 0,
            MVP = 1,
            N = 2,
            DM = 3,
            DM_ACTIVATION = 4,
            CUBE_MAP = 5,
            DIFFUSE_COLOR = 6,
            LIGHT0 = 7,
            LIGHT0_POSITION = 8,
            LIGHT0_AMBIENT = 9,
            LIGHT0_DIFFUSE = 10,
            LIGHT0_SPECULAR = 11,
            US_size = 12
        };

        ShaderProgram();
        virtual ~ShaderProgram();

    protected:
        /**
         * Creates a new ShaderProgram.
         */

        ShaderProgram(const ShaderProgram&) = delete;
        void operator=(const ShaderProgram&) = delete;


    public:
        inline GLuint getHandle() const {return mHandle;}

        inline U32 getAttributeFlags() const {
            return mAttributeFlags;
        }

        inline U32 getUniformFlags() const {
            return mUniformFlags;
        }

        inline GLint getAttributeLocation(AttribSem sem) const {
            return mAttributeLocations[sem];
        }

        inline bool hasAttribute(AttribSem sem) const {
            return (mAttributeFlags & (1L << sem)) != 0;
        }

        inline GLint getUniformLocation(UniformSem sem) const {
            return mUniformLocations[sem];
        }

        inline bool hasUniform(UniformSem sem) const {
            return (bool) (mUniformFlags & (1L << sem));
        }

        /**
         * Clear OpenGL resources
         */
        void wipe();

        inline bool hasCache() {
            return !mVertexSource.empty() && !mFragmentSource.empty();
        }

        inline void clearCache() {
            mVertexSource.clear();
            mFragmentSource.clear();
        }


    protected:
        ExceptionType mLink(GLuint vertexHandle, GLuint Fragmenthandle);
        /**
         * Retrives the locations of the attributes and uniforms
         * and sets up the flags accordingly
         */
        Status mBindLocations();


    private:
        static const std::string attributeNames[AS_size];
        static const std::string uniformNames[US_size];
        GLuint mHandle;
        //The attribute flags
        U32 mAttributeFlags;
        // The attribute locations
        GLint mAttributeLocations[AS_size];
        // The uniform flags
        U32 mUniformFlags;
        //The uniform locations
        GLint mUniformLocations[US_size];
        std::string mVertexSource;
        std::string mFragmentSource;
    };
}

#endif /* _DMA_SHADER_PROGRAM_HPP_ */
