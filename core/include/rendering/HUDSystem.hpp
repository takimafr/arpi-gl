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


#ifndef _DMA_HUDSYSTEM_HPP_
#define _DMA_HUDSYSTEM_HPP_

#include <glm/glm.hpp>
#include <memory>
#include <resource/ResourceManager.hpp>
#include "rendering/HUDElement.hpp"

namespace dma {

    class HUDSystem {

        friend class RenderingEngine;

    public:

        HUDSystem(ResourceManager& resourceManager);
        virtual ~HUDSystem();

        void setViewport(U32 width, U32 height);

        void addHUDElement(std::shared_ptr<HUDElement> hudElement);

        inline const std::list<std::shared_ptr<HUDElement>>& getHUDElements() {
            return mHUDElements;
        }

        void unload();

    private:
        glm::mat4 mV;
        glm::mat4 mP;
        ResourceManager& mResourceManager;
        std::list<std::shared_ptr<HUDElement>> mHUDElements;
    };
}

#endif //_DMA_HUDSYSTEM_HPP_
