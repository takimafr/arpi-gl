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


#ifndef _DMA_RENDERINGCOMPONENT_HPP_
#define _DMA_RENDERINGCOMPONENT_HPP_

#include "rendering/RenderingPackage.hpp"
#include "resource/Mesh.hpp"
#include "resource/Material.hpp"

#include <vector>

namespace dma {

    class RenderingComponent {


    public:
        RenderingComponent(const glm::mat4& M,
                           std::shared_ptr<Mesh> mesh,
                           std::shared_ptr<Material> material);
        RenderingComponent(const RenderingComponent&) = delete;
        void operator=(const RenderingComponent&) = delete;
        virtual ~RenderingComponent();


        void setMesh(std::shared_ptr<Mesh> mesh);

        inline const std::shared_ptr<Mesh> getMesh() const {
            return mMesh;
        }

        inline const std::vector<RenderingPackage*>& getRenderingPackages() const {
            return mRenderingPackages;
        }

        void setMaterial(std::shared_ptr<Material> material);

    private:
        std::vector<RenderingPackage*> mRenderingPackages;
        std::shared_ptr<Mesh> mMesh;
    };
}

#endif //_DMA_RENDERINGCOMPONENT_HPP_
