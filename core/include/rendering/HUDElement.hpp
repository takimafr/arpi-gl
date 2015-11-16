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


#ifndef _DMA_HUDELEMENT_HPP_
#define _DMA_HUDELEMENT_HPP_


#include <engine/Entity.hpp>
#include <resource/Quad.hpp>

namespace dma {

    class HUDElement {

    friend class HUDSystem;
    friend class RenderingEngine;

    public:

        int x;
        int y;
        int width;
        int height;
        std::string textureSID;

    private:
        std::shared_ptr<Entity> mEntity;
    };
}

#endif //_DMA_HUDELEMENT_HPP_
