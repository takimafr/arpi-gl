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


#ifndef _DMA_RENDERINGPACKAGE_HPP_
#define _DMA_RENDERINGPACKAGE_HPP_

#include <utils/Log.hpp>
#include "resource/Mesh.hpp"
#include "resource/ShaderProgram.hpp"
#include "resource/Material.hpp"
#include "resource/Texture.hpp"
#include "glm/glm.hpp"

namespace dma {

    /**
     * Contains all information needed to be consumed by the rendering engine.
     */
    class RenderingPackage {

    public:
        RenderingPackage(const glm::mat4& M,
                         std::shared_ptr<Mesh>,
                         std::shared_ptr<Material>);

        virtual ~RenderingPackage();

        inline bool isBackToFront() const { return mMaterial->isBackToFront();}

        inline std::shared_ptr<Material> getMaterial() { return mMaterial; }

        inline void setMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }

        inline void setMaterial(std::shared_ptr<Material> material) { mMaterial = material; }

    private:
        //FIELDS
        friend class RenderingEngine;
        const glm::mat4& M;
        std::shared_ptr<Mesh> mMesh;
        std::shared_ptr<Material> mMaterial;
    };
}




#endif //_DMA_RENDERINGPACKAGE_HPP_
