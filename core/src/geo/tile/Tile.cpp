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



#include "geo/tile/Tile.hpp"

constexpr auto TAG = "Tile";

constexpr int TILE_PASS_INDEX = 0;

namespace dma {

    Tile::~Tile() {

    }


//    void Tile::setDiffuseMap(std::shared_ptr<Map> diffuseMap) {
//        getMaterial()->setDiffuseMap(diffuseMap, TILE_PASS_INDEX);
//    }
//
//
//    std::shared_ptr<Material> Tile::getMaterial() {
//        return mGeoEntity->getRenderingComponent()->getRenderingPackages()[0]->getMaterial();
//    }

    std::string Tile::id() {
        return std::to_string(x) + "/" + std::to_string(y) + "/" + std::to_string(x);
    }

    void Tile::xyz(int x, int y, int z)  {
        mCoords = LatLng(GeoUtils::tiley2lat(y, z), GeoUtils::tilex2long(x, z));

        double bottomLat = GeoUtils::tiley2lat(y + 1, z);
        double rightLng = GeoUtils::tilex2long(x + 1, z);

        mWidth = (float) GeoUtils::slc(mCoords, LatLng(mCoords.lat, rightLng));
        mHeight = (float) GeoUtils::slc(mCoords, LatLng(bottomLat, mCoords.lng));

//        for (auto ge : mGeoEntities) {
//
//        }
//        mGeoEntity->setCoords(coords);
//        mQuad->setSize(mWidth, mHeight);
//        mGeoEntity->setScale(mQuad->getScale());
//
//        // Shifts the quad position since its origin is the center
//        glm::vec3 pos = mGeoEntity->getPosition();
//        pos.x += mWidth / 2.0f;
//        pos.z += mHeight / 2.0f;
//        mGeoEntity->setPosition(pos);

        this->x = x;
        this->y = y;
        this->z = z;
    }


}
