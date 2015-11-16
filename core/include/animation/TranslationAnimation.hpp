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


#ifndef _DMA_TRANSLATION_ANIMATION_HPP_
#define _DMA_TRANSLATION_ANIMATION_HPP_

#define GLM_FORCE_CXX98
#include "glm/glm.hpp"
#include "animation/Animation.hpp"

namespace dma {

    class TranslationAnimation : public Animation {

    public:

        enum Function {
            LINEAR = 0,
            EASE = 1,
            EASE_OUT = 2
        };

        TranslationAnimation(TransformComponent& transformComponent,
                             const glm::vec3 &from,
                             const glm::vec3 &to,
                             float duration,
                             Function func,
                             bool loop,
                             bool mirror);

        TranslationAnimation(TransformComponent& transformComponent,
                             const glm::vec3 &from,
                             const glm::vec3 &to,
                             float duration,
                             Function func);

        TranslationAnimation(TransformComponent& transformComponent,
                             const glm::vec3 &from,
                             const glm::vec3 &to,
                             float duration);

        virtual void update(float dt);

        virtual void refresh(const glm::vec3& from,
                             const glm::vec3& to,
                             float duration);

    protected:
        glm::vec3 mFrom;
        glm::vec3 mTo;
        bool mMirror;
        Function mFunc;
    };
}

#endif //_DMA_TRANSLATION_ANIMATION_HPP_
