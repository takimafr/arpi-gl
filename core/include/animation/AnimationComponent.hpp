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


#ifndef _DMA_ANIMATIONCOMPONENT_HPP_
#define _DMA_ANIMATIONCOMPONENT_HPP_

#include "engine/TransformComponent.hpp"
#include "animation/TranslationAnimation.hpp"
#include "animation/SlerpAnimation.hpp"

#include <list>
#include <algorithm>
#include <memory>

namespace dma {

    /**
     * An animationComponent holds animations of an entity.
     * If an entity has an animationComponent, and this animationComponents has some pending animation,
     * this entity will be animated over engine steps.
     */
    class AnimationComponent {

    public:
        AnimationComponent(TransformComponent& transformComponent);
        AnimationComponent(const AnimationComponent&) = delete;
        AnimationComponent& operator=(const AnimationComponent&) = delete;
        virtual ~AnimationComponent();

        /**
         * Add the given animation to the animation queue.
         * @param const Animation* -
         *          the animation to add.
         */
        inline void add(std::shared_ptr<Animation> animation) {
            mAnimations.push_back(animation);
        }

        /**
         * Remove the given animation to the animation queue.
         * @param const Animation* -
         *          the animation to remove.
         */
        inline void remove(std::shared_ptr<Animation> animation) {
            mAnimations.remove(animation);
        }

        /**
         * Updates all bound animations
         */
        void update(float dt);


    private:
        friend class AnimationSystem;
        /* ***
         * FRIEND METHODS
         */

        /* ***
         * ATTRIBUTES
         */
        TransformComponent& mTransformComponent;
        std::list<std::shared_ptr<Animation>> mAnimations;
    };
}

#endif //_DMA_ANIMATIONCOMPONENT_HPP_
