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



#include <utils/GeoUtils.hpp>
#include <utils/ExceptionUtils.hpp>
#include "geo/tile/TileMap.hpp"
#include "geo/tile/Tile.hpp"

#define DEFAULT_TILE_DIFFUSE_MAP "damier"

namespace dma {

    constexpr char TileMap::TAG[];
    constexpr char TileMap::TILE_MATERIAL[];
    constexpr int TileMap::SIZE;
    constexpr int TileMap::OFFSET;
    constexpr int TileMap::ZOOM;


    bool TileMap::isInRange(int x, int y, int x0, int y0) {
        return x >= x0 - OFFSET
               && x <= x0 + OFFSET
               && y >= y0 - OFFSET
               && y <= y0 + OFFSET;
    }

    /****************************************************************************/
    /*                                 MEMBERS                                  */
    /****************************************************************************/

    TileMap::TileMap(ResourceManager& resourceManager, LatLng& geoSceneOrigin) :
            mResourceManager(resourceManager),
            mTileFactory(resourceManager, geoSceneOrigin),
            mLastX(-1),
            mLastY(-1),
            mNullCallbacks(new GeoEngineCallbacks()),
            mCallbacks(mNullCallbacks) {

    }

    TileMap::~TileMap() {
        unload();
        delete mNullCallbacks;
    }

    void TileMap::init() {
        mLastX = mLastY = -1;
        // Create TILE_MAP_SIZE * TILE_MAP_SIZE tiles
        for (int i = 0; i < SIZE * SIZE; ++i) {
            mTiles.push_back(mTileFactory.create());
        }
    }

    void TileMap::unload() {
        mRemoveAllTiles();
        mLastX = mLastY = -1;
    }

    void TileMap::update(int x0, int y0) {
        Log::trace(TAG, "Updating TileMap (%d, %d, %d)", x0, y0, ZOOM);

        //TODO check x and y bounds
        if (x0 <= OFFSET || y0 <= OFFSET) {
            return;
        }

        int z = ZOOM;
        // if update gives the same tile : skip.
        if (x0 == mLastX && y0 == mLastY) {
            return;
        }

        std::list<std::shared_ptr<Tile>> toUpdate;

        for (auto tile : mTiles) {
            if (!isInRange(tile->x, tile->y, x0, y0)) {
                toUpdate.push_back(tile);
            }
        }

        for (int x = x0 - OFFSET; x <= x0 + OFFSET; ++x) {
            for (int y = y0 - OFFSET; y <= y0 + OFFSET; ++y) {
                if (!isInRange(x, y, mLastX, mLastY)) {
                    updateTile(toUpdate.front(), x, y, z);
                    toUpdate.pop_front();
                }
            }
        }

        //mResourceManager.prune(); // unload unused resources TODO removed for buildings / tracks demo

        mLastX = x0;
        mLastY = y0;
    }

    Status TileMap::notifyTileAvailable(int x, int y, int z) {
        Log::trace(TAG, "Notifying tile available (%d, %d, %d)", x, y, z);
        std::shared_ptr<Tile> tile = findTile(x, y, z);
        if (tile == nullptr) {
            std::stringstream ss;
            ss << "Trying to set Tile Image but Tile (" << x << ", " << y << ", " << z << ") doesn't exist in the TileMap";
            Log::error(TAG, "%", ss.str().c_str());
            return STATUS_KO;
            //throw std::runtime_error(ss.str());
            //throwException(TAG, ExceptionType::NO_SUCH_ELEMENT, ss.str());
        }
        std::string sid = tileSid(x, y, z);
        std::shared_ptr<Map> diffuseMap = mResourceManager.acquireMap(sid);
        tile->setDiffuseMap(diffuseMap);
        return STATUS_OK;
    }

    //TODO use a tile pool
    void TileMap::updateTile(std::shared_ptr<Tile> tile, int x, int y, int z) {
        tile->xyz(x, y, z);


        for (const auto& kv : mStyle.getSources()) {
            auto& source = kv.second;
            source.fetch(x, y, z);

        }

        for (auto& layer : mStyle.getLayers()) {
            switch (layer.getType()) {
                case Layer::Type::EXTRUDE:
                    break;
                case Layer::Type::BACKGROUND:
                    break;
                case Layer::Type::FILL:
                    break;
                case Layer::Type::LINE:
                    break;
                case Layer::Type::SYMBOL:
                    break;
                case Layer::Type::RASTER: {
                    const std::string &source = layer.getSource();
                    std::string sid = "tiles/" + source + "/" + std::to_string(z) + "/" + std::to_string(x) + "/" + std::to_string(y);
                    std::shared_ptr<Map> diffuseMap;
                    diffuseMap = mResourceManager.acquireMap(DEFAULT_TILE_DIFFUSE_MAP);
                    if (mResourceManager.hasMap(sid)) {
                        diffuseMap = mResourceManager.acquireMap(sid);
                    } else if (!mNamespace.empty()) {
                        Log::trace(TAG, "No tile found with sid %s", sid.c_str());
                        mCallbacks->onTileRequest(x, y, z);
                    }
                    tile->setDiffuseMap(diffuseMap);
                    break;
                }
                case Layer::Type::CIRCLE:
                    break;
                default:
                    ExceptionUtils::runtime(TAG, "Unknown layer type: " + layer.getType());
            }
        }



        //Log::trace(TAG, "Tile (%d, %d, %d) updated, diffuse map: %s", x, y, z, diffuseMap->getSID().c_str());
    }

    void TileMap::mRemoveAllTiles() {
        mTiles.clear();
    }

    std::shared_ptr<Tile> TileMap::findTile(int x, int y, int z) {
        for (auto tile : mTiles) {
            if (tile->x == x && tile->y == y && tile->z == z) {
                return tile;
            }
        }
        return nullptr;
    }

    std::string TileMap::tileSid(int x, int y, int z) const {
        std::stringstream ssid;
        ssid << "tiles/";
        if (!mNamespace.empty()) {
            ssid << mNamespace << "/";
        }
        ssid << z << "/" << x << "/" << y;
        return std::move(ssid.str());
    }

    void TileMap::setNamespace(const std::string &ns) {
        Log::debug(TAG, "Setting namespace: %s", ns.c_str());
        mNamespace = ns;
        if (mTiles.front()->x != -1) { // -1 means tile map not set
            updateDiffuseMaps();
        }
    }

    void TileMap::updateDiffuseMaps() {
        for (std::shared_ptr<Tile> tile : mTiles) {
            std::string sid = tileSid(tile->x, tile->y, tile->z);
            if (mResourceManager.hasMap(sid)) {
                tile->setDiffuseMap(mResourceManager.acquireMap(sid));
            } else {
                tile->setDiffuseMap(mResourceManager.acquireMap(DEFAULT_TILE_DIFFUSE_MAP));
                mCallbacks->onTileRequest(tile->x, tile->y, tile->z);
            }
        }
    }

    void TileMap::setStyle(const Style &style) {
        mStyle = style;
        for (auto& t : mTiles) {
            updateTile(t, t->x, t->y, t->z);
        }
    }


}
