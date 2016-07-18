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


#ifndef _DMA_GEO_ENGINE_HPP_
#define _DMA_GEO_ENGINE_HPP_

#include "engine/Engine.hpp"
#include "async/TaskScheduler.hpp"
#include "geo/GeoEngineCallbacks.hpp"
#include "geo/PoiFactory.hpp"
#include "geo/GeoSceneManager.hpp"
#include "geo/Poi.hpp"


namespace dma {

    /**
     * 'Geo variation' of dma Engine, wrapped with a convenient interface.
     * @version 0.2.3
     */
    class GeoEngine {

    public:

        /* ***
         * CONSTRUCTORS
         */
        explicit GeoEngine(const std::string &);

        virtual ~GeoEngine();

        /* ***
         * DELETED METHODS
         */
        GeoEngine(const GeoEngine &) = delete;
        GeoEngine& operator=(const GeoEngine &) = delete;

        /* ***
         * LEGACY METHODS
         */

        bool init();

        virtual void refresh();

        void reload();

        void unload();

        virtual void wipe();

        virtual void step();

        void post(std::function<void()> message);

        /* ***
         * GETTERS
         */

        inline bool isInit() const {
            return mEngine.isInit();
        }

        inline bool isAbleToDraw() const {
            return mEngine.isAbleToDraw();
        }


        inline PoiFactory& getPoiFactory() {
            return mPoiFactory;
        }

        inline GeoSceneManager& getGeoSceneManager() {
            return mGeoSceneManager;
        }

        /* ***
         * SETTERS
         */
        inline void setSurfaceSize(unsigned int width, unsigned int height) {
            mEngine.setSurfaceSize(width, height);
        }

        inline void setSkyBox(const std::string& sid) {
            mGeoSceneManager.getScene().setSkyBox(sid);
        }

        inline void setSkyBoxEnabled(bool enabled) {
            mGeoSceneManager.getScene().setSkyBoxEnabled(enabled);
        }

        inline void setStyle(const std::string& style) {
            std::string json;
            Utils::bufferize(mRootDir + "/" + style + ".json", json);
            mGeoSceneManager.setStyle(json);
        }

        void setCallback(GeoEngineCallbacks* callbacks);


    public:
        std::string mRootDir;
        Engine mEngine;
        PoiFactory mPoiFactory;
        GeoSceneManager mGeoSceneManager;
        GeoEngineCallbacks *mDefaultCallbacks, *mCallbacks;
        TaskScheduler mMessageQueue;
    };

} /* namespace dma */

#endif /* _DMA_GEO_ENGINE_HPP_ */
