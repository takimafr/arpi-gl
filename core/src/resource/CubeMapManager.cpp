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
 * TextureManager.cpp
 *
 *  Created on: 26 mai 2015
 *      Author: Nicolas THIERION
 */

#include "resource/ResourceManager.hpp"

//debug tag.
constexpr auto TAG = "CubeMapManager";


namespace dma {

    //-----------------------------------------------------------------------------------------------
    CubeMapManager::CubeMapManager(const std::string& dir) {
        mDir = dir;
        Utils::addTrailingSlash(mDir);
    }


    //-----------------------------------------------------------------------------------------------
    CubeMapManager::~CubeMapManager() {
    }


    //-----------------------------------------------------------------------------------------------
    void CubeMapManager::init() {
    }


    //-----------------------------------------------------------------------------------------------
    std::shared_ptr<CubeMap> CubeMapManager::acquire(const std::string & sid) {
        if (mCubeMaps.find(sid) == mCubeMaps.end()) {
            std::shared_ptr<CubeMap> cubemap = std::make_shared<CubeMap>();
            try {
                mLoadCubeMap(cubemap, sid);
            } catch (std::runtime_error& e) {
                Log::warn(TAG, "CubeMap %s doesn't exist, returning fallback instead", sid.c_str());
                assert(false); //TODO fallback
                return nullptr;
            }
            //TODO verify cubemap->setSID(sid);
            mCubeMaps[sid] = cubemap;
        }
        return mCubeMaps[sid];
    }


    //----------------------------------------------------------------------------------------------
    void CubeMapManager::mLoadCubeMap(std::shared_ptr<CubeMap> cubeMap, const std::string &sid) {
        std::string directoryName = mDir + sid;
        cubeMap->load(directoryName);
        cubeMap->setSID(sid);
    }


    //----------------------------------------------------------------------------------------------
    void CubeMapManager::reload() {
        Log::trace(TAG, "Reloading CubeMapManager...");

        for (auto& kv : mCubeMaps) {
            std::shared_ptr<CubeMap> cubemap = kv.second;
            const std::string& sid = cubemap->getSID();
            cubemap->wipe();
            std::string filename = mDir + sid;
            cubemap->load(filename);
            cubemap->setSID(sid);
        }

        Log::trace(TAG, "CubeMapManager reloaded");
    }


    //----------------------------------------------------------------------------------------------
    void CubeMapManager::refresh() {
        Log::trace(TAG, "Refreshing CubeMapManager...");

        for (auto& kv : mCubeMaps) {
            std::shared_ptr<CubeMap> cubemap = kv.second;
            const std::string& sid = cubemap->getSID();
            //cubemap->wipe();
            std::string dirName = mDir + sid; //+ ".png";
            cubemap->refresh(dirName);
        }

        Log::trace(TAG, "CubeMapManager refreshed");
    }


    //----------------------------------------------------------------------------------------------
    void CubeMapManager::wipe() {
        Log::trace(TAG, "Wiping CubeMapManager...");

        for (auto& kv : mCubeMaps) {
            kv.second->wipe();
        }

        Log::trace(TAG, "CubeMapManager wiped");
    }


    //----------------------------------------------------------------------------------------------
    void CubeMapManager::unload() {
        Log::trace(TAG, "Unloading CubeMapManager...");

        for (auto& kv : mCubeMaps) {
                kv.second->wipe();
        }
        mCubeMaps.clear();

        Log::trace(TAG, "CubeMapManager unloaded");
    }


    //----------------------------------------------------------------------------------------------
    void CubeMapManager::update() {
        auto it = mCubeMaps.begin();
        while (it != mCubeMaps.end()) {
            if (it->second.unique()) {
                it->second->wipe();
                it = mCubeMaps.erase(it);
            } else {
                ++it;
            }
        }
    }
} /* namespace dma */
