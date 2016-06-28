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

#ifndef ARPIGL_GPURESOURCEMANAGERHANDLER_HPP
#define ARPIGL_GPURESOURCEMANAGERHANDLER_HPP

#include "resource/ResourceManagerHandler.hpp"

namespace dma {

    template<typename T>
    class GpuResourceManagerHandler : public ResourceManagerHandler<T> {
    public:

        GpuResourceManagerHandler(const std::string& localDir) :
                ResourceManagerHandler<T>(localDir)
        {}

        virtual void wipe() {
            Log::trace(this->TAG, "Wiping...");

//            this->mFallback->wipe();

            for (auto& kv : this->mResources) {
                auto resource = kv.second;
                if (resource != nullptr) {
                    resource->wipe();
                }
            }

            for (auto r : ResourceManagerHandler<T>::mAnonymousResources) {
                r->wipe();
            }

            Log::trace(this->TAG, "Wipe done.");
        }

        virtual void reload() override {
            wipe();
            ResourceManagerHandler<T>::reload();
        }
    };
}

#endif //ARPIGL_GPURESOURCEMANAGERHANDLER_HPP
