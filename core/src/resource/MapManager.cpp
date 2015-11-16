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



#include "resource/MapManager.hpp"

#define TAG "MapManager"

#define FALLBACK_MAP_SID "fallback"

namespace dma {


    //-----------------------------------------------------------------
    MapManager::MapManager(const std::string& dir) {
        mMapDir = dir;
        Utils::addTrailingSlash(mMapDir);
    }


    //-----------------------------------------------------------------
    MapManager::~MapManager() {

    }


    //-----------------------------------------------------------------
    void MapManager::init() {
        mFallbackMap = std::make_shared<Map>();
        mLoadMap(mFallbackMap, FALLBACK_MAP_SID);
    }


    //-----------------------------------------------------------------
    std::shared_ptr<Map> MapManager::acquire(const std::string &sid) {
        if (sid == FALLBACK_MAP_SID) {
            return mFallbackMap;
        }
        if (mMaps.find(sid) == mMaps.end()) {
            std::shared_ptr<Map> map = std::make_shared<Map>();
            try {
                mLoadMap(map, sid);
            } catch (std::runtime_error& e) {
                Log::warn(TAG, "Map %s doesn't exist, returning fallback instead", sid.c_str());
                return mFallbackMap;
            }
            mMaps[sid] = map;
        }
        return mMaps[sid];
    }


    //-----------------------------------------------------------------
    bool MapManager::hasResource(const std::string &sid) const {
        return Utils::fileExists(mMapDir + sid + ".png") || Utils::fileExists(mMapDir + sid + ".PNG");
    }


    //-----------------------------------------------------------------
    void MapManager::reload() {
        Log::trace(TAG, "Reloading MapManager...");

        mFallbackMap->wipe();
        mLoadMap(mFallbackMap, FALLBACK_MAP_SID);

        for (auto& kv : mMaps) {
            const std::string& sid = kv.first;
            auto map = kv.second;
            map->wipe();
            std::string filename = mMapDir + sid;
            map->load(filename);
        }

        Log::trace(TAG, "MapManager reloaded");
    }


    //-----------------------------------------------------------------
    void MapManager::refresh() {
        Log::trace(TAG, "Refreshing MapManager...");

        //mFallbackMap->wipe();
        mFallbackMap->refresh();

        for (auto& kv : mMaps) {
            const std::string& sid = kv.first;
            auto map = kv.second;
            //map->wipe();
            std::string filename = mMapDir + sid;
            map->refresh(filename);
        }

        Log::trace(TAG, "MapManager refreshed");
    }


    //-----------------------------------------------------------------
    void MapManager::wipe() {
        Log::trace(TAG, "Wiping MapManager...");

        mFallbackMap->wipe();

        for (auto& kv : mMaps) {
            kv.second->wipe();
        }

        Log::trace(TAG, "MapManager wiped");
    }


    //-----------------------------------------------------------------
    void MapManager::unload() {
        Log::trace(TAG, "Unloading MapManager...");

        mFallbackMap->wipe();
        mFallbackMap = nullptr; //release reference count

        for (auto& kv : mMaps) {
            kv.second->wipe();
        }
        mMaps.clear();

        Log::trace(TAG, "MapManager unloaded");
    }


    //-----------------------------------------------------------------
    void MapManager::update() {
        auto it = mMaps.begin();
        while (it != mMaps.end()) {
            if (it->second.unique()) {
                it->second->wipe();
                it = mMaps.erase(it);
            } else {
                ++it;
            }
        }
    }


    //----------------------------------------------------------------------------------------------
    void MapManager::mLoadMap(std::shared_ptr<Map> map, const std::string &sid) {
        std::string filename = mMapDir + sid + ".png";
        if (!Utils::fileExists(filename)) {
            Log::error(TAG, "2D texture %s doesn't exist", sid.c_str());
            throw std::runtime_error("2D texture " + sid + " doesn't exist");
        }
        map->load(filename);
    }
}
