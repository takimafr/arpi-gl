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


#ifndef _DMA_RESOURCE_MANAGER_HANDLER_HPP_
#define _DMA_RESOURCE_MANAGER_HANDLER_HPP_

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <utils/Log.hpp>
#include "common/Types.hpp"

namespace dma {
    template<typename T>
    class ResourceManagerHandler {

    public:
        virtual ~ResourceManagerHandler() {

        }

        /**
         * Load the resource identified by SID.
         * @param const std::string& -
         *              SID of the resource to load.
         * @param Status* -
         *              holds OK if the resource could be loaded.
         * @return the loaded resource.
         */
        virtual std::shared_ptr<T> acquire(const std::string& sid) {
            if (mResources.find(sid) == mResources.end()) {
                std::shared_ptr<T> resource = std::make_shared<T>();
                load(resource, sid);
                mResources[sid] = resource;
            }
            return mResources[sid];
        }

        /**
         * Clear all resources used but keep the pointer to them.
         * The resource pointer remains valid, but the resource itself is not.
         * This is typically used when you need to refresh the resources
         * for example when the OpenGL context has changed.
         */
        virtual void unload() {
            Log::trace(TAG, "Unloading...");
            mResources.clear();
            mAnonymousResources.clear();
            Log::trace(TAG, "Unloaded done...");
        }

        /**
         * Refresh all resources from cache
         */
        virtual void refresh() {
            Log::trace(TAG, "Refreshing...");

            for (auto& kv : mResources) {
                const std::string& sid = kv.first;
                auto resource = kv.second;
                load(resource, sid);
            }

            for (auto r : mAnonymousResources) {
                load(r, "_anonymous_");
            }

            Log::trace(TAG, "Refresh done.");
        }

        /**
         * Reload all resources from file
         */
        virtual void reload() {
            Log::trace(TAG, "Reloading...");
            for (auto& kv : mResources) {
                auto resource = kv.second;
                resource->clearCache();
            }
            refresh();
            Log::trace(TAG, "Reload done.");
        }


        /**
         * Unload and remove not used resources
         */
        virtual void prune() {
            auto it = mResources.begin();
            while (it != mResources.end()) {
                if (it->second.unique()) {
                    it = mResources.erase(it);
                } else {
                    ++it;
                }
            }

            auto ar = mAnonymousResources.begin();
            while (ar != mAnonymousResources.end()) {
                if (ar->unique()) {
                    ar = mAnonymousResources.erase(ar);
                } else {
                    ++ar;
                }
            }
        }


    protected:
//        static constexpr auto FALLBACK_SID = "fallback";
        static constexpr auto TAG = "ResourceManagerHandler";

        ResourceManagerHandler(const std::string& localDir) :
                mLocalDir(localDir)
        {}

        /**
         * Load the resource identified by sid
         */
        virtual void load(std::shared_ptr<T> resource, const std::string& sid) = 0;

        //std::shared_ptr<T> mFallback; //TODO remove fallback ?
        std::map<std::string, std::shared_ptr<T>> mResources;
        std::vector<std::shared_ptr<T>> mAnonymousResources;
        std::string mLocalDir;
    };
} /* namespace dma */

#endif /* _DMA_RESOURCE_MANAGER_HANDLER_HPP_ */
