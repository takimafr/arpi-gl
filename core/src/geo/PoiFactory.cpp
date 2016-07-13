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



/*
 * PoiFactory.cpp
 *
 *  Created on: 5 juin 2015
 *      Author: excilys
 */

// std
#include <string>

// dma
#include "geo/PoiFactory.hpp"
#include "geo/GeoEngine.hpp"

#define POI_PASS 1

namespace dma {
    constexpr char TAG[] = "PoiFactory";
    constexpr char ICON_DIR[] = "icon/";



    PoiFactory::PoiFactory(GeoSceneManager& geoSceneManager, ResourceManager &resourceManager) :
            mGeoSceneManager(geoSceneManager),
            mResourceManager(resourceManager)
    {}



    PoiFactory::~PoiFactory() {
    }



    PoiFactory::Builder PoiFactory::builder() {
        return Builder(mGeoSceneManager, mResourceManager);
    }



    PoiFactory::Builder::Builder(GeoSceneManager& geoSceneManager, ResourceManager& resourceManager) :
            mGeoSceneManager(geoSceneManager),
            mResourceManager(resourceManager)
    {
    }


    PoiFactory::Builder &PoiFactory::Builder::sid(const std::string &sid) {
        mSid = sid;
        return *this;
    }



    PoiFactory::Builder &PoiFactory::Builder::shape(const std::string &shape) {
        mShape = shape;
        return *this;
    }



    PoiFactory::Builder &PoiFactory::Builder::icon(const std::string &icon) {
        mIcon = icon;
        return *this;
    }



    PoiFactory::Builder &PoiFactory::Builder::color(const Color &color) {
        mColor = color;
        return *this;
    }


    PoiFactory::Builder &PoiFactory::Builder::animation(bool animated) {
        mAnimated = animated;
        return *this;
    }



    std::shared_ptr<Poi> PoiFactory::Builder::build() {
        std::shared_ptr<Mesh> mesh = mResourceManager.acquireMesh(mShape);
        std::shared_ptr<Material> material = mResourceManager.createMaterial("poi");

        //////////////////////////////////////////////////////
        // Setup the "poi" pass
        Pass& poiPass = material->getPass(POI_PASS);
        if (mIcon.empty()) {
            poiPass.setDiffuseMapEnabled(false);
        } else {
            poiPass.setDiffuseMap(mResourceManager.acquireMap(ICON_DIR + mIcon));
        }
        poiPass.setDiffuseColor(glm::vec3(mColor.r, mColor.g, mColor.b));
        if (mesh->hasFlatNormals()) {
            poiPass.setLightingMode(Pass::Func::LIGHTING_FLAT);
        } else {
            poiPass.setLightingMode(Pass::Func::LIGHTING_SMOOTH);
        }

        return std::make_shared<Poi>(mSid, mesh, material, mAnimated, mGeoSceneManager);
    }

} /* namespace dma */