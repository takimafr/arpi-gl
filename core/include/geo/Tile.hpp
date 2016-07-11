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


#ifndef _DMA_GEO_TILE_HPP_
#define _DMA_GEO_TILE_HPP_

#include "engine/Entity.hpp"
#include "resource/Quad.hpp"
#include "LatLng.hpp"
#include "GeoEntity.hpp"

namespace dma {
    class Tile : public GeoEntity {

        friend class TileMap;
        friend class GeoSceneManager;

    public:
        Tile(std::shared_ptr<Quad> quad,
             std::shared_ptr<Material> material,
             GeoSceneManager& geoSceneManager);

        virtual ~Tile();


    private:

        inline const Quad &getQuad() const {
            return *mQuad;
        }

        inline void setSize(float width, float height) {
            mQuad->setSize(width, height);
            setScale(mQuad->getScale());
        }

        void setDiffuseMap(std::shared_ptr<Map> diffuseMap);

        std::shared_ptr<Map> getDiffuseMap();

        std::shared_ptr<Material> getMaterial();

    private:
        //FIELDS
        int x;
        int y;
        int z;
        std::shared_ptr<Quad> mQuad;
    };
}

#endif // _DMA_GEO_TILE_HPP_
