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



#include "rendering/RenderingComponent.hpp"

constexpr auto TAG = "RenderingComponenent";

namespace dma {

    /* ================= ROUTINES ========================*/


    void assertMeshMaterialCompatible(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) {

        for (U8 i = 0; i < material->getPassCount(); ++i) {
            Pass& pass = material->getPass(i);
            ///////////////////////////////////////////////////
            // Check if mesh has position element (required)
            if (!mesh->hasVertexElement(VertexElement::Semantic::POSITION)) {
                throw std::runtime_error("Transform required but mesh doesn't have position element");
            }

            if (pass.hasFunc(Pass::Func::LIGHTING_FLAT)) {
                ////////////////////////////////////////////////
                // Check if mesh has flat normal element
                if (!mesh->hasVertexElement(VertexElement::Semantic::FLAT_NORMAL)) {
                    throw std::runtime_error("Flat lighting computation required but mesh doesn't have flat normal element");
                }
            }
            if (pass.hasFunc(Pass::Func::LIGHTING_SMOOTH)) {
                ////////////////////////////////////////////////
                // Check if mesh has smooth normal element
                if (!mesh->hasVertexElement(VertexElement::Semantic::SMOOTH_NORMAL)) {
                    throw std::runtime_error("Smooth lighting computation required but mesh doesn't have smooth normal element");
                }
            }

            if (pass.hasFunc(Pass::Func::DIFFUSE_MAP)) {
                ////////////////////////////////////////////////
                // Check if mesh has uv element
                if (!mesh->hasVertexElement(VertexElement::Semantic::UV)) {
                    throw std::runtime_error("Diffuse map required but mesh doesn't have uv element");
                }
            }

            if (pass.hasFunc(Pass::Func::SCALING)) {
                ////////////////////////////////////////////////
                // Check if mesh has smooth normal element
                if (!mesh->hasVertexElement(VertexElement::Semantic::SMOOTH_NORMAL)) {
                    throw std::runtime_error("Scaling required but mesh doesn't have smooth normal element");
                }
            }
        }
    }

    /* ================= PUBLIC ========================*/


    RenderingComponent::RenderingComponent(const glm::mat4& M,
                                           std::shared_ptr<Mesh> mesh,
                                           std::shared_ptr<Material> material) :
            mMesh(mesh)
    {
        // Check requirements
        assertMeshMaterialCompatible(mesh, material);
        RenderingPackage* rp = new RenderingPackage(M, mesh, material);
        mRenderingPackages.push_back(rp);
    }



    RenderingComponent::~RenderingComponent() {
        for (RenderingPackage* rp : mRenderingPackages) {
            delete rp;
        }
    }



    void RenderingComponent::setMesh(std::shared_ptr<Mesh> mesh) {
        mRenderingPackages[0]->setMesh(mesh); //TODO handle multiple submeshes
    }



    void RenderingComponent::setMaterial(std::shared_ptr<Material> material) {
        mRenderingPackages[0]->setMaterial(material);
    }
}
