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

#include "resource/IResourceManager.hpp"
#include "resource/ShaderProgram.hpp"
#include "common/Types.hpp"

namespace dma {

    class ShaderManager : public IResourceManager<ShaderProgram> {

        friend class ResourceManager;

    public:
        virtual ~ShaderManager();

        /**
         * Loads the fallback shader
         */
        Status init();

        /**
         * @param const std::string& -
         *              SID of the shader to load.
         * @param Status* -
         *              holds Status::OK if the shader could be loaded.
         * @return the loaded shader, or default shader if none could be loaded.
         */
        std::shared_ptr<ShaderProgram> acquire(const std::string& sid);

        /**
         * From disk
         */
        Status reload();

        /**
         * From cache if any
         */
        Status refresh();

        /**
         * Unload all the shader programs from the GPU and clean all resources.
         */
        void unload();

        void update() override;


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
        Status mLoad(std::shared_ptr<ShaderProgram> shaderProgram, const std::string& sid) const;
        /**
         * Loads the shader program corresponding to the sid from disk
         * and uploads it to the GPU.
         * If cache is not a nullptr it will be filled with the vertex and fragment source code
         */
        //Status mLoad(ShaderProgram* shaderProgram, const std::string& sid, Cache* cache) const;

        /**
        * Loads the shader program from the provided sources and uploads it to the GPU.
        */
        Status mLoad(std::shared_ptr<ShaderProgram> shaderProgram,
                     const std::string& sid,
                     const std::string& vertexSource,
                     const std::string& fragmentSource) const;
        /**
         * Compiles the source given in parameter as GL_VERTEX_SHADER
         * or GL_FRAGMENT_SHADER according to the type parameter.
         * Returns the OpenGL shader handle
         **/
        GLuint mCompile(const std::string &source, GLenum type) const;

    private:
        static const std::string FALLBACK_SHADER_SID;

        std::map<std::string, std::shared_ptr<ShaderProgram>> mShaderPrograms;
        std::shared_ptr<ShaderProgram> mFallbackShaderProgram;
        std::string mLocalDir;
    };

}

#endif /* _DMA_SHADER_MANAGER_HPP_ */
