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

#include <utils/GeoUtils.hpp>
#include "engine/Entity.hpp"
#include "resource/Quad.hpp"
#include "geo/LatLng.hpp"
#include "geo/GeoEntity.hpp"

namespace dma {
    class Tile {

        friend class TileMap;
        friend class TileFactory;
        friend class GeoSceneManager;

    public:
//        Tile(std::shared_ptr<Quad> quad,
//             std::shared_ptr<Material> material,
//             GeoSceneManager& geoSceneManager);

        virtual ~Tile();


    private:

        void xyz(int x, int y, int z);

//        inline void setSize(float width, float height) {
//            //TODO for each mEntities -> scale
////            mQuad->setSize(width, height);
////            setScale(mQuad->getScale());
//        }

        void setDiffuseMap(std::shared_ptr<Map> diffuseMap);

        std::shared_ptr<Material> getMaterial();

    public:
        //FIELDS
        int x;
        int y;
        int z;
    private:
        float mWidth;
        float mHeight;
        std::shared_ptr<GeoEntity> mGeoEntity;
        std::shared_ptr<Quad> mQuad;
        //        std::vector<std::shared_ptr<GeoEntity>> mGeoEntities;
    };
}

#endif // _DMA_GEO_TILE_HPP_
