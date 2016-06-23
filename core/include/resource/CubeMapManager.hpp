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


#ifndef _DMA_CUBEMAPMANAGER_HPP_
#define _DMA_CUBEMAPMANAGER_HPP_

#include <string>
#include <map>

#include "resource/Map.hpp"
#include "resource/CubeMap.hpp"
#include "resource/GpuResourceManagerHandler.hpp"

namespace dma {

    class CubeMapManager : public GpuResourceManagerHandler<CubeMap> {

    public:
        CubeMapManager(const std::string& localDir);
        CubeMapManager(const CubeMapManager&) = delete;
        CubeMapManager& operator=(const CubeMapManager&) = delete;

    private:
        void load(std::shared_ptr<CubeMap> cubeMap, const std::string& sid);
    };
} /* namespace dma */

#endif /* _DMA_CUBEMAPMANAGER_HPP_ */
