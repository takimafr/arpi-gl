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


#ifndef _DMA_SHADER_MANAGER_HPP_
#define _DMA_SHADER_MANAGER_HPP_

#include <utils/GLES2Logger.hpp>

#include <string>
#include <map>

#include "resource/ResourceManagerHandler.hpp"
#include "resource/ShaderProgram.hpp"
#include "common/Types.hpp"

namespace dma {

    class ShaderManager : public ResourceManagerHandler<ShaderProgram> {

        friend class ResourceManager;

    public:
        virtual ~ShaderManager();

        /**
         * From disk
         */
        Status reload();

        /**
         * From cache if any
         */
        Status refresh();


        /**
         * Clean all GPU resources
         */
        void wipe();


        virtual bool hasResource(const std::string &) const;


    protected:
        ShaderManager(const std::string& rootDir);
        ShaderManager(const ShaderManager&) = delete;
        void operator=(const ShaderManager&) = delete;

    private:
        /**
         * Loads the shader program corresponding to the sid from disk
         * and uploads it to the GPU.
         * This is the non-cached version: the shader source code won't remain in RAM
         **/
        Status load(std::shared_ptr<ShaderProgram> shaderProgram, const std::string& sid) const;

        /**
        * Loads the shader program from the provided sources and uploads it to the GPU.
        */
        Status load(std::shared_ptr<ShaderProgram> shaderProgram,
                     const std::string& sid,
                     const std::string& vertexSource,
                     const std::string& fragmentSource) const;
        /**
         * Compiles the source given in parameter as GL_VERTEX_SHADER
         * or GL_FRAGMENT_SHADER according to the type parameter.
         * Returns the OpenGL shader handle
         **/
        GLuint compile(const std::string &source, GLenum type) const;

    private:
        std::string mLocalDir;
    };

}

#endif /* _DMA_SHADER_MANAGER_HPP_ */
