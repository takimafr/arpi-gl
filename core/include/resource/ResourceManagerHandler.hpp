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
#include <map>
#include "common/Types.hpp"

namespace dma {
    template <class T>
    class ResourceManagerHandler {
    public:
        virtual ~ResourceManagerHandler();

        virtual void init();

        /**
         * Load the resource identified by SID.
         * @param const std::string& -
         *              SID of the resource to load.
         * @param Status* -
         *              holds OK if the resource could be loaded.
         * @return the loaded resource.
         */
        virtual std::shared_ptr<T> acquire(const std::string& sid);

        /**
         * Clear all resources used but keep the pointer to them.
         * The resource pointer remains valid, but the resource itself is not.
         * This is typically used when you need to refresh the resources
         * for example when the OpenGL context has changed.
         */
        virtual void unload();

        /**
         * Unload and remove not used resources
         */
        virtual void prune();


    protected:
        constexpr auto FALLBACK_SID = "fallback";

        /**
         * Load the resource identified by sid
         */
        virtual Status load(std::shared_ptr<T> resource, std::string sid) = 0;

        std::shared_ptr<T> mFallback;
        std::map<std::string, std::shared_ptr<T>> mResources;

    };
} /* namespace dma */

#endif /* _DMA_RESOURCE_MANAGER_HANDLER_HPP_ */
