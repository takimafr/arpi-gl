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


#ifndef _DMA_ANIMATIONSYSTEM_HPP_
#define _DMA_ANIMATIONSYSTEM_HPP_

#include "common/Timer.hpp"
#include "animation/AnimationComponent.hpp"
#include "animation/Animation.hpp"
#include "animation/SlerpAnimation.hpp"
#include "animation/TranslationAnimation.hpp"

#include <list>

namespace dma {
    class AnimationSystem {


    public:
        AnimationSystem();

        void unload();

        void subscribe(AnimationComponent& animationComponent);
        void unsubscribe(AnimationComponent& animationComponent);

        void step(float dt);

    private:
        std::list<Animation*> mAnimations;
        std::list<AnimationComponent*> mAnimationComponents;
    };
}

#endif //_DMA_ANIMATIONSYSTEM_HPP_
