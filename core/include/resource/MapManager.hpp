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


#ifndef ARPIGL_MAPMANAGER_HPP
#define ARPIGL_MAPMANAGER_HPP

#include <string>
#include <memory>
#include <map>

#include "resource/Map.hpp"

namespace dma {

    class MapManager {

    public:
        MapManager(const std::string& dir);
        virtual ~MapManager();

        MapManager(const MapManager&) = delete;
        MapManager& operator=(const MapManager&) = delete;

        void init();
        std::shared_ptr<Map> acquire(const std::string& sid);
        bool hasResource(const std::string& sid) const;

        void reload();
        void refresh();
        void wipe();
        void unload();
        void update();

    private:
        void mLoadMap(std::shared_ptr<Map>, const std::string& sid);

        std::map<std::string, std::shared_ptr<Map>> mMaps;
        std::shared_ptr<Map> mFallbackMap;
        std::string mMapDir;
    };
}

#endif //ARPIGL_MAPMANAGER_HPP
