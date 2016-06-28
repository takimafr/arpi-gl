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



#include "engine/geo/GeoSceneManager.hpp"
#include "engine/geo/Tile.hpp"
#include "utils/GeoSceneReader.hpp"
#include "engine/geo/Poi.hpp"

#include <utils/GeoUtils.hpp>

#define ORIGIN_SHIFTING_TRESHOLD 8000 // 8km
#define ZOOM_LEVEL 20

constexpr float ANIMATE_CAMERA_TRANSLATION_DURATION = 0.9f;
constexpr float ANIMATE_CAMERA_ROTATION_DURATION = 0.08f;

namespace dma {
    namespace geo {
        constexpr char TAG[] = "GeoSceneManager";


        GeoSceneManager::GeoSceneManager(Scene& scene, ResourceManager& resourceManager) :
                mResourceManager(resourceManager),
                mScene(scene),
                mTileMap(*this, resourceManager),
                mLastX(-1),
                mLastY(-1)
        {
            // Add a default camera to the scene
            mScene.setCamera(std::make_shared<Camera>());
        }


        GeoSceneManager::~GeoSceneManager() {
        }


        void GeoSceneManager::init() {
            mTileMap.init();
            for (std::shared_ptr<Tile> tile : mTileMap.getTiles()) {
                mScene.addEntity(tile);
            }
        }


        void GeoSceneManager::unload() {
            Log::trace(TAG, "Unloading GeoSceneManager...");
            for (std::shared_ptr<Tile> tile : mTileMap.getTiles()) {
                mScene.removeEntity(tile);
            }
            mTileMap.unload();
            mScene.unload();
            mGeoEntities.clear();
//            mPOIs.clear();
            //removeAllPois();
            mOrigin = LatLng(0.0, 0.0);
            Log::trace(TAG, "GeoSceneManager unloaded");
        }


        void GeoSceneManager::step() { //TODO optimization ?
//            for (auto& kv : mGeoEntities) {
//                if (auto poi = std::static_pointer_cast<Poi>(kv.second)) {
//                    poi->animate();
//                }
//            }
            for (auto tile : mTileMap.getTiles()) {
                if (tile->isDirty()) {
                    LatLngAlt coords = tile->getCoords();
                    double lat = coords.lat;
                    double lon = coords.lng;
                    glm::vec3 dest = mapPosition(lat, lon, 0.0);
                    dest.x = dest.x + (tile->getQuad().getWidth() / 2.0f);
                    dest.z = dest.z + (tile->getQuad().getHeight() / 2.0f);
                    tile->setPosition(dest);
                    tile->setDirty(false);
                }
            }
        }


        std::shared_ptr<GeoEntity> GeoSceneManager::createGeoEntity(const std::string &meshSid,
                                                                    const std::string &materialSid) {
            auto mesh = mResourceManager.acquireMesh(meshSid);
            auto mat = mResourceManager.acquireMaterial(materialSid);
            auto geoEntity = createGeoEntity(mesh, mat);
            return geoEntity;
        }

        std::shared_ptr<GeoEntity> GeoSceneManager::createGeoEntity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) {
            return std::make_shared<GeoEntity>(mesh, material, *this);
        }



        void GeoSceneManager::addGeoEntity(const std::string& sid, std::shared_ptr<GeoEntity> geoEntity) {
            if (mGeoEntities.find(sid) != mGeoEntities.end()) {
                Log::warn(TAG, "GeoScene already contains GeoEntity with SID = %s. Will replace the old by the new", sid.c_str());
                mScene.removeEntity(mGeoEntities[sid]);
                mGeoEntities.erase(sid);
            }
            LatLngAlt coords = geoEntity->getCoords();
            int x = GeoUtils::lng2tilex(coords.lng, ZOOM_LEVEL);
            int y = GeoUtils::lat2tiley(coords.lat, ZOOM_LEVEL);
            mGeoEntities[sid] = geoEntity;
            if (TileMap::isInRange(x, y, mLastX, mLastY)) {
                Log::debug(TAG, "Adding GeoEntity %s", sid.c_str());
                mScene.addEntity(geoEntity);
            }
        }


        void GeoSceneManager::removeGeoEntity(const std::string& sid) {
            std::shared_ptr<GeoEntity> geoEntity = mGeoEntities[sid];
            if (geoEntity == nullptr) {
                Log::warn(TAG, "Trying to remove GeoEntity with SID = %s from the GeoScene that does not exist", sid.c_str());
                return;
            }
            mScene.removeEntity(geoEntity);
            mGeoEntities.erase(sid);
        }


        std::shared_ptr<GeoEntity> GeoSceneManager::getGeoEntity(const std::string &sid) {
            if (mGeoEntities.find(sid) == mGeoEntities.end()) {
                Log::warn(TAG, "GeoEntity %s not in the GeoScene", sid.c_str());
                return nullptr;
            }
            return mGeoEntities[sid];
        }


        bool GeoSceneManager::hasGeoEntity(const std::string &sid) {
            return mGeoEntities.find(sid) != mGeoEntities.end();
        }


        glm::vec3 GeoSceneManager::mapPosition(double lat, double lon, double alt) const {
            double bearing = GeoUtils::bearing(LatLng(lat, lon), mOrigin);
            double distance = GeoUtils::slc(LatLng(lat, lon), mOrigin);
            glm::vec3 dest = destinationPoint(bearing, distance);
            dest.y = (float) alt;
            return dest;
        }


        glm::vec3 GeoSceneManager::mapPosition(const LatLngAlt &coords) const {
            return mapPosition(coords.lat, coords.lng, coords.alt);
        }


        void GeoSceneManager::setOrigin(double lat, double lng) {
            Log::trace(TAG, "Setting new Origin: old=(%f, %f) new=(%f, %f)",
                       mOrigin.lat, mOrigin.lng, lat, lng);
            mOrigin = LatLng(lat, lng);

            // Updates scene objects
            for (auto& e : mScene.getEntities()) {
                auto ge = std::static_pointer_cast<GeoEntity>(e);
                ge->setCoords(ge->getCoords());
            }

            for (auto tile : mTileMap.getTiles()) {//TODO refacto
                tile->setDirty(true);
            }
        }


        Status GeoSceneManager::notifyTileAvailable(int x, int y, int z) {
            return mTileMap.notifyTileAvailable(x, y, z);
        }



        void GeoSceneManager::setCallbacks(GeoEngineCallbacks* callbacks) {
            mTileMap.setCallbacks(callbacks);
        }


        glm::vec3 GeoSceneManager::destinationPoint(double bearing, double distance) const {
            glm::vec3 res;
            double theta = (glm::half_pi<double>() - glm::radians(bearing));
            res.x = (float) (distance * cos(theta));
            res.y = 0.0f;
            res.z = (float) (-distance * sin(theta));
            return res;
        }


        void GeoSceneManager::placeCamera(const LatLng& coords) {
            placeCamera(LatLngAlt(coords.lat, coords.lng, mScene.getCamera().getPosition().y));
        }



        void GeoSceneManager::placeCamera(const LatLngAlt& coords) {
            placeCamera(coords, -1.0f, TranslationAnimation::Function::LINEAR);
        }



        void GeoSceneManager::placeCamera(const LatLngAlt& coords, float translationDuration, TranslationAnimation::Function translationFunction) {
            Camera &camera = mScene.getCamera();

            int x0 = GeoUtils::lng2tilex(coords.lng, ZOOM_LEVEL);
            int y0 = GeoUtils::lat2tiley(coords.lat, ZOOM_LEVEL);

            if (x0 != mLastX or y0 != mLastY) {
                if (GeoUtils::slc(LatLng(coords.lat, coords.lng), mOrigin) > ORIGIN_SHIFTING_TRESHOLD) {
                    setOrigin(coords.lat, coords.lng);
                    // Update the current camera position with no translation
                    camera.setPosition(mapPosition(mCameraCoords));
                }
                mTileMap.update(x0, y0);

                for (auto &kv : mGeoEntities) {
//                    const std::string& sid = kv.first;
                    std::shared_ptr<GeoEntity> geoEntity = kv.second;
                    LatLngAlt& geoEntityCoords = geoEntity->getCoords();
                    int x = GeoUtils::lng2tilex(geoEntityCoords.lng, ZOOM_LEVEL);
                    int y = GeoUtils::lat2tiley(geoEntityCoords.lat, ZOOM_LEVEL);
                    bool isInRange = TileMap::isInRange(x, y, x0, y0);
                    bool wasInRange = TileMap::isInRange(x, y, mLastX, mLastY);
                    if (wasInRange and !isInRange) {
                        mScene.removeEntity(geoEntity);
                        //mGeoEntities.erase(sid);
                    } else if (!wasInRange and isInRange) {
                        mScene.addEntity(geoEntity);
                    }
                }
            }

            glm::vec3 pos = mapPosition(coords.lat, coords.lng, coords.alt);
//            if (!TileMap::isInRange(mLastX, mLastY, x0, y0)) {
//                translationDuration = -1.0f;
//            }
            camera.setPosition(pos, translationDuration, translationFunction);
            mCameraCoords = coords;
            mLastX = x0;
            mLastY = y0;
        }



        void GeoSceneManager::orientateCamera(std::shared_ptr<glm::mat4> rotationMatrix) {
            mScene.getCamera().setOrientation(*rotationMatrix, ANIMATE_CAMERA_ROTATION_DURATION);
        }



        void GeoSceneManager::setTileNamespace(const std::string &ns) {
            mTileMap.setNamespace(ns);
        }



        std::shared_ptr<Poi> GeoSceneManager::pick(int screenX, int screenY) {
            std::list<std::shared_ptr<Poi>> intersected;
            glm::vec3 ray = mScene.castRay(screenX, screenY);

            for (auto& kv : mGeoEntities) {
                std::shared_ptr<Poi> poi = std::dynamic_pointer_cast<Poi>(kv.second);
                if (poi == nullptr) continue;
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
            if (mSelected != nullptr && mSelected->getSid() != closest->getSid()) {
                mSelected->getMaterial()->getPass(0).setDiffuseColor(glm::vec3(0.0f, 0.0f, 0.0f));
                mTileMap.mCallbacks->onPoiDeselected(mSelected->getSid()); //TODO see TODO below
            }
            mSelected = closest;

            // Notify listener TODO keep reference and use shared pointer
            mTileMap.mCallbacks->onPoiSelected(mSelected->getSid());

            return closest;
        }


//        void GeoSceneManager::addTrack(const std::string &sid, const std::vector<LatLngAlt>& geoPoints, float thickness, const Color &color) {
//            // Map geo points to scene coordinates
//            std::vector<glm::vec3> points;
//            for (auto& p : geoPoints) {
//                points.push_back(mapPosition(p.lat, p.lng, p.alt));
//            }
//            std::shared_ptr<Entity> track = mTrackFactory.generate(points, thickness, color);
//            mTracks[sid] = track;
//            mScene.addEntity(track);
//        }
//
//
//        void GeoSceneManager::removeTrack(const std::string &sid) {
//            std::shared_ptr<Entity> track = mTracks[sid];
//            if (track == nullptr) {
//                Log::warn(TAG, "Track %s not found", sid.c_str());
//                return;
//            }
//            mScene.removeEntity(track);
//        }


        void GeoSceneManager::updateTileDiffuseMaps() {
            mTileMap.updateDiffuseMaps();
        }
    }
}


