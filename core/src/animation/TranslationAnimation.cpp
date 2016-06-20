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



#include "animation/TranslationAnimation.hpp"

namespace dma {


    float linear(float x, bool mirror) {
        if (mirror) {
            if (x <= 0.5) {
                return x * 2;
            } else {
                return (1-x) * 2;
            }
        } else {
            return x;
        }

    }



    float ease(float x, bool mirror) {
        if (mirror) {
            x *= 2;
            if (x > 1.0f) {
                x = (2.0f - x);
            }
        }
        float a = x * x;
        float b = 1.0f - x;
        float c = b * b;
        return a / (a + c);
    }


    float easeOut(float x, bool mirror) {
        return -x*(x-2);
    }



    float backAndForth(float x) {
        return  1.0f - ((float) ((cos(2 * M_PI * x) + 1.0f) / 2.0f));
    }



    TranslationAnimation::TranslationAnimation(TransformComponent& transformComponent,
                                               const glm::vec3 &from,
                                               const glm::vec3 &to,
                                               float duration,
                                               Function func,
                                               bool loop,
                                               bool mirror) :
            Animation(transformComponent, duration, loop),
            mFrom(from),
            mTo(to),
            mMirror(mirror),
            mFunc(func)
    {}



    TranslationAnimation::TranslationAnimation(TransformComponent &transformComponent,
                                               const glm::vec3 &from,
                                               const glm::vec3 &to,
                                               float duration,
                                               Function func) :
            TranslationAnimation(transformComponent, from, to, duration,
                                 func, false, false)
    {}



    TranslationAnimation::TranslationAnimation(TransformComponent &transformComponent,
                                               const glm::vec3 &from,
                                               const glm::vec3 &to,
                                               float duration) :
            TranslationAnimation(transformComponent, from, to, duration,
                                 Function::LINEAR)
    {}



    void TranslationAnimation::update(float dt) {
        mCurrentTime += dt;
        if (mLoop) {
            mCurrentTime = glm::mod<float>(mCurrentTime, mDuration);
        } else if (mCurrentTime > mDuration) {
            mCurrentTime = mDuration;
        }

        float x = mCurrentTime / mDuration;
        float interpolant;
        switch (mFunc) {
            case LINEAR:
                interpolant = linear(x, mMirror);
                break;
            case EASE:
                interpolant = ease(x, mMirror);
                break;
            case EASE_OUT:
                interpolant = easeOut(x, mMirror);
                break;
            default:
                assert(false);
                return;
        }
        mTransformComponent->setPosition(glm::mix(mFrom, mTo, interpolant));
    }



    void TranslationAnimation::refresh(const glm::vec3 &from, const glm::vec3 &to, float duration) {
        mFrom = from;
        mTo = to;
        mDuration = duration;
        mCurrentTime = 0.0f; //reset the current time
    }
}
