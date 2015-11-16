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



#include "animation/RotationAnimation.hpp"

namespace dma {


    //-------------------------------------------------------------------------
    RotationAnimation::RotationAnimation(TransformComponent &transformComponent,
            float duration, bool loop,
            float angle, const glm::vec3 &axis) :
                        Animation(transformComponent, duration, loop),
                        mAngle(angle),
                        mCurrentAngle(0.0f),
                        mAxis(axis)
    {}


    //-------------------------------------------------------------------------
    RotationAnimation::~RotationAnimation() {

    }


    //-------------------------------------------------------------------------
    void RotationAnimation::update(float dt) {
        mCurrentTime += dt;
        if (mLoop) {
            mCurrentTime = glm::mod<float>(mCurrentTime, mDuration);
        } else if (mCurrentTime > mDuration) {
            mCurrentTime = mDuration;
        }

        float newAngle = mAngle * mCurrentTime / mDuration;
        float da = glm::mod<float>(newAngle - mCurrentAngle, 360.0f);
        mCurrentAngle = newAngle;

        mTransformComponent->rotate(da, mAxis);
    }

}
