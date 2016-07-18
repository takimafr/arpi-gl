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


#ifndef _DMA_GEO_TILEMAP_HPP_
#define _DMA_GEO_TILEMAP_HPP_

#include "geo/GeoEngineCallbacks.hpp"
#include "resource/ResourceManager.hpp"
#include "Style.hpp"
#include <geo/tile/TileFactory.hpp>


#include <list>
#include <queue>

namespace dma {
    class Tile;
//    class TileFactory

    class TileMap {

        /* ****
         * CONSTANTS
         */
        static constexpr char TAG[] = "TileMap";
        static constexpr char TILE_MATERIAL[] = "tile";
        static constexpr int SIZE = 13;
        static constexpr int OFFSET = SIZE / 2;
        static constexpr int ZOOM = 20;


    public:

        TileMap(ResourceManager& resourceManager, LatLng& geoSceneOrigin);
        TileMap(const TileMap&) = delete;
        void operator=(const TileMap&) = delete;
        virtual ~TileMap();

        /* ***
         * STATIC TOOL METHODS
         */
        static bool isInRange(int x, int y, int x0, int y0);

        /* ***
         * PUBLIC METHODS
         */
        void init();

        /**
         * call unload() on each tile to clean memory.
         */
        void unload();

        void setStyle(const Style& style);

        inline GeoEngineCallbacks* getCallbacks() {
            return mCallbacks;
        }

        inline std::list<std::shared_ptr<Tile>>& getTiles() {
            return mTiles;
        }

        /**
         * Constructs and displays tiles close to the location (lat, lon)
         */
        void update(int x0, int y0);

        /**
         * Notify that a tmp png provided is available
         * @return Status::OK if tile could be loaded.
         */
        Status notifyTileAvailable(int x, int y, int z);

        void setNamespace(const std::string& ns);

        void setCallbacks(GeoEngineCallbacks* callbacks) {
            if(!callbacks) {
                mCallbacks = mNullCallbacks;
            } else {
                mCallbacks = callbacks;
            }
        }

        void updateDiffuseMaps();

    private:
        //METHODS

        std::string tileSid(int x, int y, int z) const;

        void updateTile(std::shared_ptr<Tile> tile, int x, int y, int z);


        void mRemoveAllTiles();

        std::shared_ptr<Tile> findTile(int x, int y, int z);

        //Fields
        ResourceManager& mResourceManager;
        TileFactory mTileFactory;
        /** the last known center position. */
        int mLastX, mLastY;
        std::list<std::shared_ptr<Tile>> mTiles;
        std::string mNamespace;
        GeoEngineCallbacks* mNullCallbacks, * mCallbacks;
        Style mStyle;
    };
}

#endif //_DMA_GEO_TILEMAP_HPP_
