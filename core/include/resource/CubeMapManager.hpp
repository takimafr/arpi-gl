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

#include "resource/TextureManager.hpp"
#include "resource/Map.hpp"
#include "resource/CubeMap.hpp"

namespace dma {

    class CubeMapManager {

    public:
        CubeMapManager(const std::string& dir);
        virtual ~CubeMapManager();

        CubeMapManager(const CubeMapManager&) = delete;
        CubeMapManager& operator=(const CubeMapManager&) = delete;

        void init();
        std::shared_ptr<CubeMap> acquire(const std::string& sid);

        void reload();
        void refresh();
        void wipe();
        void unload();
        void update();

    private:
        void mLoadCubeMap(std::shared_ptr<CubeMap> cubeMap, const std::string& sid);

        std::map<std::string, std::shared_ptr<CubeMap>> mCubeMaps;
        std::string mDir;
    };
} /* namespace dma */

#endif /* _DMA_CUBEMAPMANAGER_HPP_ */
