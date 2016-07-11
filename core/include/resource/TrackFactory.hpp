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
#include <geo/LatLngAlt.hpp>
#include "resource/MeshManager.hpp"
#include "resource/ResourceManager.hpp"
#include "utils/Color.hpp"

namespace dma {

    class TrackFactory {
    public:
        class Builder;
        class PathBuilder;

        TrackFactory(ResourceManager& resourceManager);

        std::shared_ptr<Mesh> generateMesh(const std::vector<glm::vec3>& points, float thickness) const;
        std::shared_ptr<Material> generateMaterial(const Color& color) const;

        Builder builder();

        class Builder {
            friend class PathBuilder;
            friend class TrackFactory;
        public:
            Builder& thickness(float thickness);
            Builder& color(float r, float g, float b);
            PathBuilder path();
        protected:
            Builder(const TrackFactory& trackFactory);
            const TrackFactory& mTrackFactory;
            Color mColor;
            float mThickness;
        };

        class PathBuilder {
            friend class Builder;
        public:
            PathBuilder& point(const glm::vec3& point);
            std::shared_ptr<Mesh> build();
        protected:
            PathBuilder(const Builder& builder);
            const Builder& mBuilder;
            std::vector<glm::vec3> mPoints;
        };

    private:
        ResourceManager& resourceManager;
    };
}

#endif //ARPIGL_TRACK_HPP
