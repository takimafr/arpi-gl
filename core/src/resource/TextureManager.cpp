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

#include <cassert>


#include "resource/ResourceManager.hpp"
#include "utils/ExceptionHandler.hpp"

//debug tag.
constexpr auto TAG = "TextureManager";


namespace dma {

    const std::string TextureManager::FALLBACK_TEXTURE_SID = "fallback";


    //----------------------------------------------------------------------------------------------
    TextureManager::TextureManager(const std::string& localDir) {
        mLocalDir = localDir;
        Utils::addTrailingSlash(mLocalDir);
    }


    //----------------------------------------------------------------------------------------------
    TextureManager::~TextureManager() {
    }


    //----------------------------------------------------------------------------------------------
    Status TextureManager::init() {
        Status result;
        mFallbackTexture = std::make_shared<Map>();
        result = mLoadMap(mFallbackTexture, FALLBACK_TEXTURE_SID);
        assert(result == STATUS_OK);
        return result;
    }


    //----------------------------------------------------------------------------------------------
    std::shared_ptr<Texture> TextureManager::acquire(const std::string & sid) {

        if (sid == FALLBACK_TEXTURE_SID) {
            return mFallbackTexture;
        }

        if (mTextures.find(sid) == mTextures.end()) {
            std::shared_ptr<Map> map = std::make_shared<Map>();
            if (mLoadMap(map, sid) != STATUS_OK) {
                Log::warn(TAG, "Map %s doesn't exist, returning fallback instead", sid.c_str());
                return mFallbackTexture;
            }
            mTextures[sid] = map;
        }
        return mTextures[sid];
    }



    //----------------------------------------------------------------------------------------------
    Status TextureManager::reload() {
        Log::trace(TAG, "Reloading TextureManager...");

        if (mFallbackTexture != nullptr) {
            mFallbackTexture->wipe();
            mLoadMap(mFallbackTexture, FALLBACK_TEXTURE_SID);
        }

        for (auto& kv : mTextures) {
            const std::string& sid = kv.first;
            auto& texture = kv.second;
            if (texture != nullptr) {
                texture->wipe();
                std::string filename = mLocalDir + sid;// + ".png";
                if (texture->load(filename) != STATUS_OK) {
                    Log::error(TAG, "Error while reloading texture %s", sid.c_str());
                    assert(false);
                    return STATUS_KO;
                }
            }
        }

        Log::trace(TAG, "TextureManager reloaded");
        return STATUS_OK;
    }


    //----------------------------------------------------------------------------------------------
    Status TextureManager::refresh() {
        Log::trace(TAG, "Refreshing TextureManager...");

        if (mFallbackTexture != nullptr) {
            mFallbackTexture->wipe();
            mFallbackTexture->refresh();
        }

        for (auto& kv : mTextures) {
            const std::string& sid = kv.first;
            auto texture = kv.second;
            if (texture != nullptr) {
                texture->wipe();
                std::string filename = mLocalDir + sid; //+ ".png";
                if (texture->refresh(filename) != STATUS_OK) {
                    Log::error(TAG, "Error while refreshing texture %s", sid.c_str());
                    assert(false);
                    return STATUS_KO;
                }
            }
        }

        Log::trace(TAG, "TextureManager refreshed");
        return STATUS_OK;
    }


    //----------------------------------------------------------------------------------------------
    void TextureManager::unload() {
        Log::trace(TAG, "Unloading TextureManager...");

        if (mFallbackTexture != nullptr) {
            mFallbackTexture->wipe();
            mFallbackTexture = nullptr; //release reference count
        }

        for (auto& kv : mTextures) {
            if (kv.second != nullptr) {
                kv.second->wipe();
            }
        }
        mTextures.clear();

        Log::trace(TAG, "TextureManager unloaded");
    }


    //----------------------------------------------------------------------------------------------
    void TextureManager::wipe() {
        Log::trace(TAG, "Wiping TextureManager...");

        if (mFallbackTexture != nullptr) {
            mFallbackTexture->wipe();
        }

        for (auto& kv : mTextures) {
            if (kv.second != nullptr) {
                kv.second->wipe();
            }
        }

        Log::trace(TAG, "TextureManager wiped");
    }


    //----------------------------------------------------------------------------------------------
    bool TextureManager::hasResource(const std::string & sid) const {
        /* deduce filename from sid */
        return Utils::fileExists(mLocalDir + sid + ".png") || Utils::fileExists(mLocalDir + sid + ".PNG");
    }


    //----------------------------------------------------------------------------------------------
    Status TextureManager::mLoadMap(std::shared_ptr<Map> map, const std::string &sid) {
        std::string filename = mLocalDir + sid + ".png";
        if (!Utils::fileExists(filename)) {
            filename = mLocalDir + sid + ".PNG";
        }
        if (!Utils::fileExists(filename)) {
            Log::error(TAG, "2D texture %s doesn't exist", sid.c_str());
            throwError(TAG, ExceptionType::IO, ("2D texture " + sid + " doesn't exist").c_str());
            return STATUS_KO;
        }
        return map->load(filename);
    }


    //------------------------------------------------------------------------------------------------
    void TextureManager::update() {
        auto it = mTextures.begin();
        while (it != mTextures.end()) {
            if (it->second.unique()) {
                it->second->wipe();
                it = mTextures.erase(it);
            } else {
                ++it;
            }
        }
    }
} /* namespace dma */
