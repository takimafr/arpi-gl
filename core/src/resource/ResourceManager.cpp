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



#include "resource/ResourceManager.hpp"

static constexpr auto TAG = "ResourceManager";

namespace dma {

    ResourceManager::ResourceManager(const std::string& resourceDir) :
        mShaderManager(resourceDir + "/shader"),
        mMeshManager(resourceDir + "/mesh"),
        mMapManager(resourceDir + "/texture"),
        mCubeMapManager(resourceDir + "/texture/cubemap"),
        mMaterialManager(resourceDir + "/material", mShaderManager, mMapManager),
        mQuadFactory()
    {}


    void ResourceManager::init() {
        Log::trace(TAG, "Initializing ResourceManager...");
        mQuadFactory.init();
        Log::trace(TAG, "ResourceManager initialized");
    }

    Status ResourceManager::refresh() {
        Log::trace(TAG, "Refreshing ResourceManager...");
        mShaderManager.refresh();
        mMapManager.refresh();
        mCubeMapManager.refresh();
        mMeshManager.refresh();
        mQuadFactory.refresh();
        Log::trace(TAG, "ResourceManager refreshed");
        return STATUS_OK;
    }

    Status ResourceManager::reload() {
        Log::trace(TAG, "Reloading ResourceManager...");
        mShaderManager.reload();
        mMapManager.reload();
        mCubeMapManager.reload();
        mMeshManager.reload();
        mMaterialManager.reload();
        mQuadFactory.refresh();
        Log::trace(TAG, "ResourceManager reloaded");
        return STATUS_OK;
    }

    void ResourceManager::unload() {
        Log::trace(TAG, "Unloading ResourceManager...");
        wipe();
        mShaderManager.unload();
        mMeshManager.unload();
        mMapManager.unload();
        mCubeMapManager.unload();
        mMaterialManager.unload();
        mQuadFactory.unload();
        Log::trace(TAG, "ResourceManager unloaded");
    }

    void ResourceManager::wipe() {
        Log::trace(TAG, "Wiping ResourceManager...");
        mShaderManager.wipe();
        mMeshManager.wipe();
        mMapManager.wipe();
        mCubeMapManager.wipe();
        mQuadFactory.wipe();
        Log::trace(TAG, "ResourceManager wiped");
    }

    void ResourceManager::prune() {
        Log::trace(TAG, "Pruning ResourceManager...");
        mMaterialManager.prune();
        mMeshManager.prune();
        mMapManager.prune();
        mCubeMapManager.prune();
        mShaderManager.prune();
        Log::trace(TAG, "ResourceManager pruned");
    }
}
