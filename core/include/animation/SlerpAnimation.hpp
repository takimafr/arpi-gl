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


#ifndef _DMA_SLERPANIMATION_HPP_
#define _DMA_SLERPANIMATION_HPP_

#define GLM_FORCE_CXX98
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "animation/Animation.hpp"


namespace dma {

    /**
     * Do a spherical linear interpolation from an orientation to another.
     */
    class SlerpAnimation : public Animation {

    public:
        SlerpAnimation(TransformComponent& transformComponent,
                const glm::quat &from,
                const glm::quat &to,
                float duration,
                bool loop);

        void update(float dt);

        void refresh(const glm::quat& from, const glm::quat& to, float duration);

    private:
        /* ***
         * ATTRIBUTES
         */
        glm::quat mFrom;
        glm::quat mTo;
    };
}

#endif //_DMA_SLERPANIMATION_HPP_
