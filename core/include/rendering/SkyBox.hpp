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


#ifndef _DMA_SKYBOX_HPP
#define _DMA_SKYBOX_HPP

#include "resource/CubeMap.hpp"
#include "rendering/VertexBuffer.hpp"
#include "rendering/IndexBuffer.hpp"

#include <memory>


namespace dma {

    class SkyBox {

    public:

        SkyBox();
        SkyBox(const SkyBox&) = delete;
        SkyBox& operator=(const SkyBox&) = delete;
        virtual ~SkyBox();

        void init(std::shared_ptr<CubeMap> cubeMap, std::shared_ptr<ShaderProgram> shaderProgram);
        void refresh();
        void wipe();

        inline const std::shared_ptr<CubeMap> getCubeMap() const { return mCubeMap; }
        inline void setCubeMap(std::shared_ptr<CubeMap> cubeMap) { mCubeMap = cubeMap; }

        inline const VertexBuffer& getVertexBuffer() const { return mVertexBuffer; }
        inline const IndexBuffer& getIndexBuffer() const { return mIndexBuffer; }

        inline std::shared_ptr<ShaderProgram> getShaderProgram() const { return mShaderProgram; }

    private:
        std::shared_ptr<CubeMap> mCubeMap;
        VertexBuffer mVertexBuffer;
        IndexBuffer mIndexBuffer;
        std::shared_ptr<ShaderProgram> mShaderProgram;
    };
}

#endif //_DMA_SKYBOX_HPP
