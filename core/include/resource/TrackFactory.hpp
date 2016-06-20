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

#ifndef ARPIGL_TRACK_HPP
#define ARPIGL_TRACK_HPP

#include <glm/vec3.hpp>
#include <vector>
#include <engine/Entity.hpp>
#include "MeshManager.hpp"
#include "ResourceManager.hpp"
#include "engine/geo/GeoSceneManager.hpp"

using namespace dma::geo;

namespace dma {

    class TrackFactory {
    public:

        TrackFactory(GeoSceneManager& geoSceneManager, ResourceManager& resourceManager);

        std::shared_ptr<Entity> generate(const std::vector<LatLngAlt>& geoPoints, float thickness);

    private:
        GeoSceneManager& geoSceneManager;
        ResourceManager& resourceManager;
    };
}

#endif //ARPIGL_TRACK_HPP
