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



#include "animation/SlerpAnimation.hpp"

namespace dma {

    //---------------------------------------------------------------------------
    SlerpAnimation::SlerpAnimation(TransformComponent& transformComponent,
            const glm::quat &from,
            const glm::quat &to,
            float duration,
            bool loop) :
                        Animation(transformComponent, duration, loop),
                        mFrom(from),
                        mTo(to)
    {}


    //---------------------------------------------------------------------------
    void SlerpAnimation::update(float dt) {
        mCurrentTime += dt;
        if (mLoop) {
            mCurrentTime = glm::mod<float>(mCurrentTime, mDuration);
        } else if (mCurrentTime > mDuration) {
            mCurrentTime = mDuration;
        }

        float interpolant = mCurrentTime / mDuration;
        glm::mat4 orientation = glm::mat4_cast(glm::slerp(mFrom, mTo, interpolant));
        mTransformComponent->setOrientation(orientation);
    }


    //---------------------------------------------------------------------------
    void SlerpAnimation::refresh(const glm::quat &from, const glm::quat &to, float duration) {
        mFrom = from;
        mTo = to;
        mDuration = duration;
        mCurrentTime = 0.0f; //refresh current time
    }
}
