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



#include "rendering/HUDSystem.hpp"

#define HUD_ELEMENT_MATERIAL "hud"

namespace dma {

    //----------------------------------------------------------------------------
    HUDSystem::HUDSystem(ResourceManager& resourceManager) :
            mV(glm::mat4(1.0f)),
            mP(glm::mat4(1.0f)),
            mResourceManager(resourceManager)
    {}


    //----------------------------------------------------------------------------
    HUDSystem::~HUDSystem() {
        unload();
    }


    //----------------------------------------------------------------------------
    void HUDSystem::setViewport(U32 width, U32 height) {
//        mP = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
        mP = glm::ortho(0.0f ,(float)width, 0.0f, (float)height);
    }


    //----------------------------------------------------------------------------
    void HUDSystem::addHUDElement(std::shared_ptr<HUDElement> hudElement) {

        std::shared_ptr<Quad> quad = mResourceManager.createQuad(hudElement->width, hudElement->height);
        std::shared_ptr<Material> mat = mResourceManager.acquireMaterial(HUD_ELEMENT_MATERIAL);
        mat->getPass(0).setDiffuseMap(mResourceManager.acquireMap(hudElement->textureSID));

        std::shared_ptr<Entity> entity = std::make_shared<Entity>(quad, mat);
        entity->setPosition(glm::vec3(hudElement->x + hudElement->width / 2.0f, hudElement->y - hudElement->height / 2.0f, 0.0f));
        entity->setScale(quad->getScale());
        entity->update(0.0f);
        hudElement->mEntity = entity;
        mHUDElements.push_back(hudElement);
    }


    //----------------------------------------------------------------------------
    void HUDSystem::unload() {
        mHUDElements.clear();
    }

}
