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



#include <fstream>
#include <resource/Watermark.hpp>
#include <utils/LocationReader.hpp>
#include "engine/geo/GeoEngine.hpp"
#include "engine/geo/GeoSceneManager.hpp"

constexpr char TAG[] = "PoiEngine";

namespace dma {
    namespace geo {

        //------------------------------------------------------------------------------
        GeoEngine::GeoEngine(const std::string &resourceDir) :
                mRootDir((!resourceDir.empty() && resourceDir.at(resourceDir.length() - 1) != '/') ? resourceDir + '/' : resourceDir),
                mEngine(resourceDir),
                mPoiFactory(mEngine.getResourceManager()),
                mGeoSceneManager(mEngine.getScene(), mEngine.getResourceManager()),
                mDefaultCallbacks(new GeoEngineCallbacks()),
                mCallbacks(mDefaultCallbacks)
        {}


        //------------------------------------------------------------------------------
        GeoEngine::~GeoEngine() {

        }


        //------------------------------------------------------------------------------
        bool GeoEngine::init() {
            bool res = mEngine.init();
            mGeoSceneManager.init();

            std::ofstream out(mRootDir + "texture/watermark.png", std::ios::binary);
            out.write((const char *) Watermark::DATA, Watermark::SIZE);
            out.close();

            std::shared_ptr<HUDElement> watermark = std::make_shared<HUDElement>();
            watermark->x = 20;
            watermark->y = 200 + 20;
            watermark->width = 200;
            watermark->height = 200;
            watermark->textureSID = "watermark";
            //mEngine.addHUDElement(watermark);

            return res;
        }


        //------------------------------------------------------------------------------
        void GeoEngine::refresh() {
            mEngine.refresh();
        }


        //------------------------------------------------------------------------------
        void GeoEngine::reload() {
            std::ofstream out(mRootDir + "texture/watermark.png", std::ios::binary);
            out.write((const char *) Watermark::DATA, Watermark::SIZE);
            out.close();
            mEngine.reload();
        }


        //------------------------------------------------------------------------------
        void GeoEngine::unload() {
            mGeoSceneManager.unload();
            mEngine.unload();
        }


        //------------------------------------------------------------------------------
        void GeoEngine::wipe() {
            mEngine.wipe();
        }


        //------------------------------------------------------------------------------
        void GeoEngine::step() {
            mMessageQueue.flush();
            mGeoSceneManager.step();
            mEngine.step();
        }


        //------------------------------------------------------------------------------
        void GeoEngine::post(std::function<void()> message) {
            mMessageQueue << message;
        }

        //------------------------------------------------------------------------------
        void GeoEngine::setCallback(GeoEngineCallbacks* callbacks) {
            if (!callbacks) {
                mCallbacks = mDefaultCallbacks;
            } else {
                mCallbacks = callbacks;
            }
            mGeoSceneManager.setCallbacks(callbacks);
        }
    } /* namespace geo */
} /* namespace dma */

