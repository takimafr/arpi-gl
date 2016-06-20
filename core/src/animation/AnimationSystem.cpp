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



#include "animation/AnimationSystem.hpp"
#include "utils/Log.hpp"

constexpr float ROTATION_RATE        = 90.0f;
constexpr float TRANSLATION_LENGTH   = 2.0f;
constexpr float TRANSLATION_SPEED    = 3.0f;

constexpr char TAG[] = "AnimationSystem";

namespace dma {


    AnimationSystem::AnimationSystem()
    {}



    void AnimationSystem::unload() {
        assert(false); //TODO remove animation system
        mAnimations.clear();
        mAnimationComponents.clear();
    }



    void AnimationSystem::subscribe(AnimationComponent &animationComponent) {
        assert(false); //TODO remove animation system
        mAnimationComponents.push_back(&animationComponent);
    }



    void AnimationSystem::unsubscribe(AnimationComponent &animationComponent) {
        assert(false); //TODO remove animation system
        mAnimationComponents.remove(&animationComponent);
    }



    void AnimationSystem::step(float dt) {
        assert(false); //TODO remove animation system
        for (AnimationComponent* ac : mAnimationComponents) {
            ac->update(dt);
        }

    }
}
