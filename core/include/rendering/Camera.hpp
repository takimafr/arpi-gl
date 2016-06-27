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


#ifndef _DMA_CAMERA_HPP_
#define _DMA_CAMERA_HPP_

#include <animation/AnimationComponent.hpp>
#include <animation/RotationAnimation.hpp>
#include "common/Types.hpp"
#include "engine/TransformComponent.hpp"
#include "rendering/Frustum.hpp"

#include "glm/glm.hpp"

namespace dma {
    class Camera {
    public:

        /* ***
         * CONSTANTS
         */
        /** default vertical field of view, in degrees. */
        static constexpr float DEFAULT_FOVY                 = 45.0f;
        /** default aspect ratio. */
        static constexpr float DEFAULT_ASPECT_RATIO         = 1.0f;
        static constexpr float DEFAULT_NEAR_PLANE           = 0.3f;
        static constexpr float DEFAULT_FAR_PLANE            = 500.0f;


        /* ***
         * CONSTRUCTORS
         */
        Camera();
        virtual ~Camera();


        /* ***
         * GETTERS
         */
        /**
         * Get the AnimationComponent of this camera.
         */
        inline AnimationComponent& getAnimationComponent() {
            return mAnimationComponent;
        }

        inline TransformComponent& getTransformComponent() {
            return mTransformComponent;
        }

        /**
         * @return the position of this camera.
         */
        inline const glm::vec3& getPosition() {
            return mTransformComponent.getPosition();
        }

        /* ***
         * SETTERS
         */

        /**
         * Set the camera perspective.
         * @param float -
         *          the vertical field of view.
         * @param float -
         *          the aspect ratio.
         * @param float -
         *          the near-place distance.
         * @param float -
         *          the far-place distance.
         */
        void setPerspective(F32 fovy = DEFAULT_FOVY,
                            F32 aspect = DEFAULT_ASPECT_RATIO,
                            F32 zNear = DEFAULT_NEAR_PLANE,
                            F32 zFar = DEFAULT_FAR_PLANE);

        /**
         * Set the camera Aspect ratio
         * @param float -
         *          the aspect ratio.
         */
        inline void setAspectRatio(F32 aspect) {
            mFrustum.setAspectRatio(aspect);
        }

        /**
         * Set the camera field-of-view
         * @param float -
         *          the vertical field of view.
         */
        inline void setFovY(F32 fovy) {
            mBaseFov = fovy;
            mFrustum.setFovY(fovy);
        }

        inline bool containsSphere(const glm::vec3& center, float radius) {
            return mFrustum.containsSphere(center, radius);
        }


        virtual void setPosition(const glm::vec3& position);
        virtual void setPosition(const glm::vec3& position, float duration);

        /**
         * Set the camera position
         * @param const glm::vec3& -
         *      the camera new position.
         * @param float -
         *      duration of the translation animation. if <= 0, setPosition take effect immediately.
         */
        virtual void setPosition(const glm::vec3& position, float duration, TranslationAnimation::Function translationFunction);

        inline glm::mat4 getOrientation() { return mTransformComponent.getOrientationMatrix(); }


        /**
         * Set the camera rotation
         * @param const glm::quat& -
         *      the camera new orientation.
         * @param float -
         *      duration of the rotation animation. if <= 0, rotation take effect immediately.
         */
        void setOrientation(const glm::quat& orientation, float duration = 0.0f);

        inline void setOrientation(const glm::mat4& orientation, float duration = 0.0f) {
            setOrientation(glm::quat_cast(orientation), duration);
        }


        void translate(const glm::vec3& translation);
        void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

        /**
         * Changes the zoom level
         * offset must be between [-1.0, 1.0]
         * if |offset| > 1.0f this will clamp the value to 1.0 -1.0
         * a positive offset will zoom in
         * a negative offset will zoom out
         */
        void zoom(float offset);



        inline const glm::mat4& getView() const {return mView;}
        inline const glm::mat4& getProjection() const { return mFrustum.getProjection();}

        /**
         * Updates the camera view matrix
         */
        virtual void update(float dt);


    protected:
        /* ***
         * ATTRIBUTES
         */
        TransformComponent mTransformComponent;
        AnimationComponent mAnimationComponent;
        TranslationAnimation* mCurrentTranslationAnimation; //TODO smart pointer
        SlerpAnimation* mCurrentSlerpAnimation; //TODO smart pointer
        glm::mat4 mView;
        float mZoom;
        float mBaseFov;
        Frustum mFrustum;
        bool mDirty;
    };
}

#endif //_DMA_CAMERA_HPP_
