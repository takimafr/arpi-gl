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



#include "resource/MapManager.hpp"

constexpr auto TAG = "MapManager";

namespace dma {

    MapManager::MapManager(const std::string& localDir) :
        GpuResourceManagerHandler(localDir)
    {}


    bool MapManager::hasResource(const std::string &sid) const {
        return Utils::fileExists(mLocalDir + "/" + sid + ".png");
    }


    void MapManager::load(std::shared_ptr<Map> map, const std::string &sid) {
        std::string filename = mLocalDir + "/" + sid + ".png";
        if (!Utils::fileExists(filename)) {
            Log::error(TAG, "2D texture %s doesn't exist", sid.c_str());
            throw std::runtime_error("2D texture " + sid + " doesn't exist");
        }
        map->load(filename);
    }
}
