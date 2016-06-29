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


#ifndef _DMA_PATH_ANIMATION_HPP_
#define _DMA_PATH_ANIMATION_HPP_

#define GLM_FORCE_CXX98

#include <vector>
#include <common/Types.hpp>
#include "glm/glm.hpp"
#include "animation/Animation.hpp"

namespace dma {

    class PathAnimation : public Animation {

    public:

        PathAnimation(TransformComponent& transformComponent,
                      std::vector<glm::vec3>& path,
                      float speed,
                      bool loop);

        virtual void update(float dt);

    protected:
        std::vector<glm::vec3> mPath;
        std::vector<float> mDurations;
        float mSpeed;
    };
}

#endif //_DMA_PATH_ANIMATION_HPP_
