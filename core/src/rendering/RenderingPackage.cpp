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



#include "rendering/RenderingPackage.hpp"

namespace dma {



        /* ================= PUBLIC ========================*/

        RenderingPackage::RenderingPackage(const glm::mat4& M,
                                           std::shared_ptr<Mesh> mesh,
                                           std::shared_ptr<Material> material)  :
                M(M),
                mMesh(mesh),
                mMaterial(material)
        {}


        RenderingPackage::~RenderingPackage() {

    }
}
