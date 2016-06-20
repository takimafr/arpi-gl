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


#ifndef _DMA_IRESOURCEMANAGER_HPP_
#define _DMA_IRESOURCEMANAGER_HPP_

#include <string>
#include <memory>
#include "common/Types.hpp"
//#include <boost/std::shared_ptr.hpp>

//using namespace boost;

namespace dma {
    template <class T>
    class IResourceManager {
    public:
        virtual ~IResourceManager() {}

        virtual Status init() = 0;

        /**
         * Load the resource identifiedby SID.
         * @param const std::string& -
         *              SID of the resource to load.
         * @param Status* -
         *              holds OK if the resource could be loaded.
         * @return the loaded resource.
         */
        virtual std::shared_ptr<T> acquire(const std::string&) = 0;
        /**
         * @param const std::string& -
         *              SID of the resource to load.
         */
        virtual void unload() = 0;

        virtual void update() = 0;

        /**
         * param const std::string& SID of the resource to test.
         * @return true if there is a resource that matches the given SID.
         */
        virtual bool hasResource(const std::string &) const = 0;


    };
} /* namespace dma */

#endif /* _DMA_IRESOURCEMANAGER_HPP_ */
