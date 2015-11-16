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
#include "engine/geo/GeoSceneManager.hpp"
#include "utils/GeoSceneReader.hpp"

#include <utils/GeoUtils.hpp>

#define ORIGIN_SHIFTING_TRESHOLD 8000 // 8km
#define ZOOM_LEVEL 19

constexpr float ANIMATE_CAMERA_TRANSLATION_DURATION = 0.9f;
constexpr float ANIMATE_CAMERA_ROTATION_DURATION = 0.08f;

namespace dma {
    namespace geo {
        constexpr char TAG[] = "GeoSceneManager";

        //------------------------------------------------------------------------------
        GeoSceneManager::GeoSceneManager(Scene& scene, ResourceManager& resourceManager) :
                mScene(scene),
                mTileMap(resourceManager),
                mLastX(-1),
                mLastY(-1)
        {
            // Add a default camera to the scene
            mScene.setCamera(std::make_shared<Camera>());
        }


        //------------------------------------------------------------------------------
        GeoSceneManager::~GeoSceneManager() {
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::init() {
            mTileMap.init();
            for (std::shared_ptr<Tile> tile : mTileMap.getTiles()) {
                mScene.addEntity(tile);
            }
        }

        //------------------------------------------------------------------------------
        void GeoSceneManager::unload() {
            Log::trace(TAG, "Unloading GeoSceneManager...");
            for (std::shared_ptr<Tile> tile : mTileMap.getTiles()) {
                mScene.removeEntity(tile);
            }
            mTileMap.unload();
            removeAllPois();
            mOrigin.lat = 0.0;
            mOrigin.lng = 0.0;
            Log::trace(TAG, "GeoSceneManager unloaded");
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::step() { //TODO optimization ?
            for (auto& kv : mPOIs) {
                auto poi = kv.second;
                if (poi->isDirty()) {
                    const glm::vec3 pos = computePosition(poi->getLat(), poi->getLng(), poi->getAlt());
                    std::static_pointer_cast<Entity>(poi)->setPosition(pos);
                    poi->animate();
                    poi->setDirty(false);
                }
            }

            for (auto tile : mTileMap.getTiles()) {
                if (tile->isDirty()) {
                    double lat = tile->getLat();
                    double lon = tile->getLng();
                    glm::vec3 dest = computePosition(lat, lon, 0.0);
                    dest.x = dest.x + (tile->getQuad().getWidth() / 2.0f);
                    dest.z = dest.z + (tile->getQuad().getHeight() / 2.0f);
                    tile->setPosition(dest);
                    tile->setDirty(false);
                }
            }
        }


        //------------------------------------------------------------------------------
        glm::vec3 GeoSceneManager::computePosition(double lat, double lon, double alt) const {
            double bearing = GeoUtils::bearing(LatLng(lat, lon), mOrigin);
            double distance = GeoUtils::slc(LatLng(lat, lon), mOrigin);
            glm::vec3 dest = destinationPoint(bearing, distance);
            dest.y = (float) alt;
            return dest;
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::setOrigin(double lat, double lon) {
            Log::trace(TAG, "Setting new Origin: old=(%f, %f) new=(%f, %f)",
                       mOrigin.lat, mOrigin.lng, lat, lon);
            mOrigin = LatLng(lat, lon);

            for (auto& kv : mPOIs) {
                kv.second->setDirty(true);
            }

            for (auto tile : mTileMap.getTiles()) {
                tile->setDirty(true);
            }
        }

        //------------------------------------------------------------------------------
        Status GeoSceneManager::notifyTileAvailable(int x, int y, int z) {
            return mTileMap.notifyTileAvailable(x, y, z);
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::setCallbacks(GeoEngineCallbacks* callbacks) {
            mTileMap.setCallbacks(callbacks);
        }


        //------------------------------------------------------------------------------
        bool GeoSceneManager::addPoi(std::shared_ptr<Poi> poi) {
            if (hasPoi(poi->getSid())) {
                Log::warn(TAG, "GeoScene already contains Poi with SID = %s", poi->getSid().c_str());
                return false;
            }
            int x = GeoUtils::lng2tilex(poi->getLng(), ZOOM_LEVEL);
            int y = GeoUtils::lat2tiley(poi->getLat(), ZOOM_LEVEL);
            if (!TileMap::isInRange(x, y, mLastX, mLastY)) {
                Log::warn(TAG, "Trying to add poi %s that is out of the tile map range", poi->getSid().c_str());
                return false;
            }
            Log::debug(TAG, "Adding Poi %s", poi->getSid().c_str());
            mPOIs[poi->getSid()] = poi;
            mScene.addEntity(poi);
            return true;
        }


        //------------------------------------------------------------------------------
        bool GeoSceneManager::removePoi(const std::string& sid) {
            if (mPOIs.find(sid) == mPOIs.end()) {
                Log::warn(TAG, "Trying to remove poi with SID = %s from the GeoScene that does not exist", sid.c_str());
                return false;
            }
            mScene.removeEntity(mPOIs[sid]);
            mPOIs.erase(sid);
            return true;
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::removeAllPois() {
            for (auto& kv : mPOIs) {
                mScene.removeEntity(kv.second);
            }
            mPOIs.clear();
        }


        //----------------------------------------------------------------------
        bool GeoSceneManager::hasPoi(const std::string& sid) {
            return mPOIs.find(sid) != mPOIs.end();
        }


        /* ***
         * PRIVATE
         */

        //------------------------------------------------------------------------------
        glm::vec3 GeoSceneManager::destinationPoint(double bearing, double distance) const {
            glm::vec3 res;
            double theta = (glm::half_pi<double>() - glm::radians(bearing));
            res.x = (float) (distance * cos(theta));
            res.y = 0.0f;
            res.z = (float) (-distance * sin(theta));
            return res;
        }


        //------------------------------------------------------------------------------
        std::shared_ptr<Poi> GeoSceneManager::getPoi(const std::string &sid) {
            if (mPOIs.find(sid) == mPOIs.end()) {
                Log::warn(TAG, "No poi found with the sid %s", sid.c_str());
                return nullptr;
            }
            return mPOIs[sid];
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::placeCamera(const LatLng& coords) {
            placeCamera(LatLngAlt(coords.lat, coords.lng, mScene.getCamera().getPosition().y));
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::placeCamera(const LatLngAlt& coords) {
            placeCamera(LatLngAlt(coords.lat, coords.lng, coords.alt), -1.0f, TranslationAnimation::Function::LINEAR);
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::placeCamera(const LatLngAlt& coords, float translationDuration, TranslationAnimation::Function translationFunction) {

            Camera &camera = mScene.getCamera();

            int x0 = GeoUtils::lng2tilex(coords.lng, ZOOM_LEVEL);
            int y0 = GeoUtils::lat2tiley(coords.lat, ZOOM_LEVEL);

            if (x0 != mLastX or y0 != mLastY) {

                if (GeoUtils::slc(LatLng(coords.lat, coords.lng), mOrigin) > ORIGIN_SHIFTING_TRESHOLD) {
                    setOrigin(coords.lat, coords.lng);
                    // Update the current camera position with no translation
                    camera.setPosition(computePosition(mCameraCoords.lat, mCameraCoords.lng, mCameraCoords.alt));
                }
                mTileMap.update(x0, y0);

                for (auto &kv : mPOIs) {
                    std::shared_ptr<Poi> poi = kv.second;
                    int x = GeoUtils::lng2tilex(poi->getLng(), ZOOM_LEVEL);
                    int y = GeoUtils::lat2tiley(poi->getLat(), ZOOM_LEVEL);
                    if (!TileMap::isInRange(x, y, x0, y0)) {
                        mScene.removeEntity(poi);
                        mPOIs.erase(poi->getSid());
                    } else {
                        poi->setDirty(true);
                    }
                }
            }

            glm::vec3 pos = computePosition(coords.lat, coords.lng, coords.alt);
            if (!TileMap::isInRange(mLastX, mLastY, x0, y0)) {
                translationDuration = -1.0f;
            }
            camera.setPosition(pos, translationDuration, translationFunction);
            mCameraCoords = coords;
            mLastX = x0;
            mLastY = y0;
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::orientateCamera(std::shared_ptr<glm::mat4> rotationMatrix) {
            mScene.getCamera().setOrientation(*rotationMatrix, ANIMATE_CAMERA_ROTATION_DURATION);
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::setTileNamespace(const std::string &ns) {
            mTileMap.setNamespace(ns);
        }


        //------------------------------------------------------------------------------
        std::shared_ptr<Poi> GeoSceneManager::pick(int screenX, int screenY) {
            std::list<std::shared_ptr<Poi>> intersected;
            glm::vec3 ray = mScene.castRay(screenX, screenY);

            for (auto& kv : mPOIs) {
                std::shared_ptr<Poi> poi = kv.second;
                if (poi->intersects(ray, mScene.getCamera().getPosition())) {
                    intersected.push_back(poi);
                }
            }

            if (intersected.empty()) {
                if (mSelected != nullptr) {
                    mSelected->getMaterial()->getPass(0).setDiffuseColor(glm::vec3(0.0f, 0.0f, 0.0f));
                    mTileMap.mCallbacks->onPoiDeselected(mSelected->getSid()); //TODO shared pointer etc... see TODO below
                    mSelected = nullptr;
                }
                return nullptr;
            }

            std::shared_ptr<Poi> closest = intersected.back();
            intersected.pop_back();
            float distance = mScene.distanceFromCamera(closest);

            while (!intersected.empty()) {
                float d =  mScene.distanceFromCamera(intersected.back());
                if (d < distance) {
                    closest = intersected.back();
                    distance = d;
                }
                intersected.pop_back();
            }

            //Log::debug(TAG, "closest: %s", closest->getSid().c_str());


            closest->getMaterial()->getPass(0).setDiffuseColor(glm::vec3(0.8f, 0.1f, 0.3f));
            if (mSelected != nullptr) {
                mSelected->getMaterial()->getPass(0).setDiffuseColor(glm::vec3(0.0f, 0.0f, 0.0f));
                mTileMap.mCallbacks->onPoiDeselected(mSelected->getSid()); //TODO see TODO below
            }
            mSelected = closest;

            // Notify listener TODO keep reference and use shared pointer
            mTileMap.mCallbacks->onPoiSelected(mSelected->getSid());

            return closest;
        }


        //------------------------------------------------------------------------------
        void GeoSceneManager::updateTileDiffuseMaps() {
            mTileMap.updateDiffuseMaps();
        }
    }
}


