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


#ifndef _DMA_ROTATIONANIMATION_HPP_
#define _DMA_ROTATIONANIMATION_HPP_

#include "animation/Animation.hpp"

namespace dma {

    class RotationAnimation : public Animation {
    public:
        /**
         * axis must be normalized
         */
        RotationAnimation(TransformComponent &transformComponent,
                float duration, bool loop, float angle,
                const glm::vec3& axis);

        virtual ~RotationAnimation();

        void update(float dt) override;

    private:
        float mAngle;
        float mCurrentAngle;
        glm::vec3 mAxis;
    };
}

#endif //_DMA_ROTATIONANIMATION_HPP_
