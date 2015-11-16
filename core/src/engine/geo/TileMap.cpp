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
#include <string.h>
#include "utils/Utils.hpp"
#include "engine/geo/TileMap.hpp"

#define DEFAULT_TILE_DIFFUSE_MAP "damier"

namespace dma {
    namespace geo {

        constexpr char TileMap::TAG[];
        constexpr char TileMap::TILE_MATERIAL[];
        constexpr int TileMap::SIZE;
        constexpr int TileMap::OFFSET;
        constexpr int TileMap::ZOOM;

        //---------------------------------------------------------------------------
        bool TileMap::isInRange(int x, int y, int xp, int yp) {
            return x >= xp - OFFSET
                   && x <= xp + OFFSET
                   && y >= yp - OFFSET
                   && y <= yp + OFFSET;
        }

        /****************************************************************************/
        /*                                 MEMBERS                                  */
        /****************************************************************************/


        //---------------------------------------------------------------------------
        TileMap::TileMap(ResourceManager& resourceManager) :
                mResourceManager(resourceManager),
                mLastX(-1),
                mLastY(-1),
                mNullCallbacks(new GeoEngineCallbacks()),
                mCallbacks(mNullCallbacks) {

        }


        //---------------------------------------------------------------------------
        TileMap::~TileMap() {
            unload();
            delete mNullCallbacks;
        }


        //---------------------------------------------------------------------------
        void TileMap::init() {
            mLastX = mLastY = -1;
            // Create TILE_MAP_SIZE * TILE_MAP_SIZE tiles
            for (int i = 0; i < SIZE * SIZE; ++i) {
                std::shared_ptr<Quad> quad = mResourceManager.createQuad(1.0f, 1.0f);
                Status status;
                std::shared_ptr<Material> mat = mResourceManager.createMaterial(TILE_MATERIAL, &status); //material with default tile texture
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(quad, mat);
                //TODO remove set in material tile.json tile->setDiffuseMap(mResourceManager.acquireTexture(DEFAULT_TILE_DIFFUSE_MAP, &status));
                tile->mDirty = true;
                mTiles.push_back(tile);
            }
        }


        //---------------------------------------------------------------------------
        void TileMap::unload() {
            mRemoveAllTiles();
            mLastX = mLastY = -1;
        }


        //---------------------------------------------------------------------------
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

            double coords[SIZE+1][SIZE+1][2];

            for (int x = x0 - OFFSET; x <= x0 + OFFSET + 1; ++x) {
                for (int y = y0 - OFFSET; y <= y0 + OFFSET + 1; ++y) {
                    double tileLat = GeoUtils::tiley2lat(y, z);
                    double tileLon = GeoUtils::tilex2long(x, z);
                    coords[x-(x0 -OFFSET)][y-(y0 -OFFSET)][0] = tileLat;
                    coords[x-(x0 -OFFSET)][y-(y0 -OFFSET)][1] = tileLon;
                }
            }

            bool isUpToDate[SIZE][SIZE];
            memset(isUpToDate, 0, SIZE*SIZE);

            std::list<std::shared_ptr<Tile>> toUpdate;

            for (auto tile : mTiles) {
                if (isInRange(tile->x, tile->y, x0, y0)) {
                    int i = tile->x - x0 + OFFSET;
                    int j = tile->y - y0 + OFFSET;
                    isUpToDate[i][j] = true;
                } else {
                    toUpdate.push_back(tile);
                }
            }

            for (int x = x0 - OFFSET; x <= x0 + OFFSET; ++x) {
                for (int y = y0 - OFFSET; y <= y0 + OFFSET; ++y) {
                    int i = x - x0 + OFFSET;
                    int j = y - y0 + OFFSET;
                    if (!isUpToDate[i][j]) {
                        std::shared_ptr<Tile> tile = toUpdate.front();
                        toUpdate.pop_front();
                        double tileLat = coords[i][j][0];
                        double tileLng = coords[i][j][1];

                        double rightTileLat = coords[i + 1][j][0];
                        double rightTileLng = coords[i + 1][j][1];

                        double bottomTileLat = coords[i][j + 1][0];
                        double bottomTileLon = coords[i][j + 1][1];

                        float width = (float) GeoUtils::slc(LatLng(tileLat, tileLng), LatLng(rightTileLat, rightTileLng));
                        float height = (float) GeoUtils::slc(LatLng(tileLat, tileLng), LatLng(bottomTileLat, bottomTileLon));
                        Status status = mUpdateTile(tile, tileLat, tileLng, width, height, x, y, z);
                        if (status != STATUS_OK) {
                            std::stringstream ss;
                            ss << "error while creating tilemap (" << x0 << "," << y0
                            << ") with tile (" << x << ", " << y << ", " << z << ")";
                            Log::error(TAG, ss.str());
                            throw std::runtime_error(ss.str());
                        }
                    }
                }
            }

            mResourceManager.update(); // unload unused resources

            mLastX = x0;
            mLastY = y0;
        }


        //---------------------------------------------------------------------------
        Status TileMap::notifyTileAvailable(int x, int y, int z) {
            Log::trace(TAG, "Notifying tile available (%d, %d, %d)", x, y, z);
            std::shared_ptr<Tile> tile = findTile(x, y, z);
            if (tile == nullptr) {
                std::stringstream ss;
                ss <<"Trying to set Tile Image but Tile (" << x << ", " << y << ", " << z << ") doesn't exist in the TileMap";
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


        //---------------------------------------------------------------------------
        Status TileMap::mUpdateTile(std::shared_ptr<Tile> tile, double lat, double lng, float width, float height, int x , int y, int z) {

            tile->x = x;
            tile->y = y;
            tile->z = z;
            tile->setSize(width, height);
            tile->mCoords.lat = lat;
            tile->mCoords.lng = lng;

            std::shared_ptr<Map> diffuseMap;

            std::string sid = tileSid(x, y, z);

            diffuseMap = mResourceManager.acquireMap(DEFAULT_TILE_DIFFUSE_MAP);
            if (mResourceManager.hasMap(sid)) {
                diffuseMap = mResourceManager.acquireMap(sid);
            } else if (!mNamespace.empty()) {
                Log::trace(TAG, "No tile found with sid %s", sid.c_str());
                mCallbacks->onTileRequest(x, y, z);
            }

            tile->setDiffuseMap(diffuseMap);
            tile->setDirty(true);
            //Log::trace(TAG, "Tile (%d, %d, %d) updated, diffuse map: %s", x, y, z, diffuseMap->getSID().c_str());
            return STATUS_OK;
        }


        //---------------------------------------------------------------------------
        void TileMap::mRemoveAllTiles() {
            mTiles.clear();
        }


        //---------------------------------------------------------------------------
        std::shared_ptr<Tile> TileMap::findTile(int x, int y, int z) {
            for (auto tile : mTiles) {
                if (tile->x == x && tile->y == y && tile->z == z) {
                    return tile;
                }
            }
            return nullptr;
        }


        //---------------------------------------------------------------------------
        std::string TileMap::tileSid(int x, int y, int z) const {
            std::stringstream ssid;
            ssid << "tiles/";
            if (!mNamespace.empty()) {
                ssid << mNamespace << "/";
            }
            ssid << z << "/" << x << "/" << y;
            return ssid.str();
        }


        //---------------------------------------------------------------------------
        void TileMap::setNamespace(const std::string &ns) {
            Log::debug(TAG, "Setting namespace: %s", ns.c_str());
            mNamespace = ns;
            if (mTiles.front()->x != -1) { // -1 means tile map not set
                updateDiffuseMaps();
            }
        }


        //---------------------------------------------------------------------------
        void TileMap::updateDiffuseMaps() {
            for (std::shared_ptr<Tile> tile : mTiles) {
                std::string sid = tileSid(tile->x, tile->y, tile->z);
                if (mResourceManager.hasMap(sid)) {
                    tile->setDiffuseMap(mResourceManager.acquireMap(sid));
                } else {
                    tile->setDiffuseMap(mResourceManager.acquireMap(DEFAULT_TILE_DIFFUSE_MAP));
                    mCallbacks->onTileRequest(tile->x, tile->y, tile->z);
                }
                tile->setDirty(true);
            }
        }
    }
}
