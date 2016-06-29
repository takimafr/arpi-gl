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



#include <glm/gtc/matrix_access.hpp>
#include "glm/gtx/quaternion.hpp"
#include "rendering/Camera.hpp"
#include "utils/Log.hpp"

#include "glm/gtc/matrix_transform.hpp"

constexpr char TAG[] = "Camera";

namespace dma {



    Camera::Camera() :
            mAnimationComponent(mTransformComponent),
            mCurrentTranslationAnimation(nullptr),
            mCurrentSlerpAnimation(nullptr)
    {
        mView = glm::lookAt(glm::vec3(0.0f, 0.0, 7.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f));
        mZoom = 1.0f;

        //Perspective values
        mBaseFov = DEFAULT_FOVY;
        setPerspective(mBaseFov, DEFAULT_ASPECT_RATIO, DEFAULT_NEAR_PLANE, DEFAULT_FAR_PLANE);
    }



    Camera::~Camera() {
//        delete mCurrentTranslationAnimation;
//        delete mCurrentSlerpAnimation;
    }



    void Camera::setPerspective(F32 fovy, F32 aspect, F32 zNear, F32 zFar) {
        mFrustum.setPerspective(fovy, aspect, zNear, zFar);
        mBaseFov = fovy;
        mZoom = 1.0f;
    }



    void Camera::setPosition(const glm::vec3& position) {
        setPosition(position, 0.0f);
    }



    void Camera::setPosition(const glm::vec3& position, float duration) {
        setPosition(position, duration, TranslationAnimation::Function::EASE);
    }



    void Camera::setPosition(const glm::vec3& position, float duration, TranslationAnimation::Function translationFunction) {
        if (duration <= 0.0) {
            //remove current animation
            if (mCurrentTranslationAnimation != nullptr) {
                mAnimationComponent.remove(mCurrentTranslationAnimation);
//                delete mCurrentTranslationAnimation;
                mCurrentTranslationAnimation = nullptr;
            }
            mTransformComponent.setPosition(position);
        } else if (mCurrentTranslationAnimation != nullptr) {
            if (mCurrentTranslationAnimation->isFinished()) {
                mAnimationComponent.add(mCurrentTranslationAnimation);
            }
            mCurrentTranslationAnimation->refresh(mTransformComponent.getPosition(),
                                                  position,
                                                  duration);
        } else {
            // translates to position
            mCurrentTranslationAnimation = std::make_shared<TranslationAnimation>(mTransformComponent,
                                                                                  mTransformComponent.getPosition(),
                                                                                  position, duration,
                                                                                  translationFunction);
            mAnimationComponent.add(mCurrentTranslationAnimation);
        }
    }




    void Camera::setOrientation(const glm::quat& orientation, float duration) {
        if (duration <= 0.0f) {
            // remove current animation
            if (mCurrentSlerpAnimation != nullptr) {
                mAnimationComponent.remove(mCurrentSlerpAnimation);
//                delete mCurrentSlerpAnimation;
                mCurrentSlerpAnimation = nullptr;
            }
            mTransformComponent.setOrientation(orientation);
        }
        else if (mCurrentSlerpAnimation != nullptr) {
            if (mCurrentSlerpAnimation->isFinished()) {
                mAnimationComponent.add(mCurrentSlerpAnimation);
            }
            mCurrentSlerpAnimation->refresh(mTransformComponent.getOrientationQuat(),
                                            orientation, duration);
        }
        else {
            // slerp the rotation
            mCurrentSlerpAnimation = std::make_shared<SlerpAnimation>(mTransformComponent,
                                                                      mTransformComponent.getOrientationQuat(),
                                                                      orientation, duration, false);
            mAnimationComponent.add(mCurrentSlerpAnimation);
        }
    }



    void Camera::translate(const glm::vec3 &translation) {
        mTransformComponent.translate(translation);
    }



    void Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
        mView = glm::lookAt(eye, center, up);
    }



    void Camera::zoom(float offset) {
        mZoom -= offset;
        if (mZoom < 0.2f) {
            mZoom = 0.2f;
        }
        else if (mZoom > 1.0f) {
            mZoom = 1.0f;
        }
        mFrustum.setFovY(mBaseFov * mZoom + (1 - mZoom));
        mDirty = true;
    }



    void Camera::update(float dt) {
        mAnimationComponent.update(dt);
        if (mDirty || mTransformComponent.isDirty()) {
            mView = mTransformComponent.getOrientationMatrix() *
                    glm::translate(glm::mat4(1.0f), -mTransformComponent.getPosition());

            glm::vec3 d = glm::vec3(0.0f, 0.0f, -1.0f) * mTransformComponent.getOrientationQuat();// * glm::vec3(1.0f, 0.0f, 0.0f);
            d = glm::normalize(d);

            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f) * mTransformComponent.getOrientationQuat();
            up = glm::normalize(up);

            mFrustum.update(mTransformComponent.getPosition(), d, up);

            mDirty = false;
            mTransformComponent.setDirty(false);
        }
    }
}
