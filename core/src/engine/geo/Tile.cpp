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



#include "engine/geo/Tile.hpp"

constexpr auto TAG = "Tile";

#define TILE_PASS_INDEX 0

namespace dma {
    namespace geo {


        Tile::Tile(std::shared_ptr<Quad> quad,
                   std::shared_ptr<Material> material,
                   const LatLng& coords, int x, int y, int z) :
                Entity(quad, material),
                mCoords(coords),
                x(x), y(y), z(z),
                mQuad(quad)
        {
            pitch(-90.0f);
            setScale(mQuad->getScale());
        }



        Tile::Tile(std::shared_ptr<Quad> quad, std::shared_ptr<Material> material) :
            Tile(quad, material, LatLng(), -1, -1, -1)
        {
        }



        Tile::~Tile() {

        }

//
//        bool Tile::operator<(const Tile& other) const {
//            return mId < other.mId;
//        }


//
//        bool Tile::operator==(const Tile& other) const {
//            return mId == other.mId;
//        }



        void Tile::setDiffuseMap(std::shared_ptr<Map> diffuseMap) {
            getMaterial()->setDiffuseMap(diffuseMap, TILE_PASS_INDEX);
        }



        std::shared_ptr<Map> Tile::getDiffuseMap() {
            return getMaterial()->getPass(TILE_PASS_INDEX).getDiffuseMap();
        }



        std::shared_ptr<Material> Tile::getMaterial() {
            return mRenderingComponent->getRenderingPackages()[0]->getMaterial();
        }
    }
}
