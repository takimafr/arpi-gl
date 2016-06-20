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



#include <rendering/FlyThroughCamera.hpp>
#include <glm/gtc/matrix_access.hpp>

#define TAG "FlyThroughCamera"

namespace dma {


    FlyThroughCamera::FlyThroughCamera() : Camera(), mYaw(-90.0f) {

    }



    FlyThroughCamera::~FlyThroughCamera() {

    }



    void FlyThroughCamera::forward(const float offset) {
        Camera::translate(mDirection * offset);
    }



    void FlyThroughCamera::backward(const float offset) {
        Camera::translate(-mDirection * offset);
    }



    void FlyThroughCamera::strafeRight(const float offset) {
        glm::vec3 right = glm::cross(glm::vec3(mDirection), glm::vec3(mUp));
        Camera::translate(right * offset);
    }



    void FlyThroughCamera::strafeLeft(const float offset) {
        glm::vec3 right = glm::cross(glm::vec3(mDirection), glm::vec3(mUp));
        Camera::translate(-right * offset);
    }



    void FlyThroughCamera::pitch(float offset) {
        mPitch += offset;
        if (mPitch > 89.0f) {
            mPitch = 89.0f;
        } else if (mPitch < -89.0f) {
            mPitch = -89.0f;
        }
        mDirty = true;
    }



    void FlyThroughCamera::yaw(const float offset) {
        mYaw = glm::mod(mYaw + offset, 360.0f);
        mDirty = true;
    }



    void FlyThroughCamera::update(float dt) {
        mAnimationComponent.update(dt);

        if (mDirty || mTransformComponent.isDirty()) {
            float cosPitch = (float) cos(glm::radians(mPitch));
            float cosPitch90 = (float) cos(glm::radians(mPitch + 90.0f));
            float sinPitch = (float) sin(glm::radians(mPitch));
            float sinPitch90 = (float) sin(glm::radians(mPitch + 90.0f));
            float cosYaw = (float) cos(glm::radians(mYaw));
            float sinYaw = (float) sin(glm::radians(mYaw));

            mDirection.x = cosYaw * cosPitch;
            mDirection.y = sinPitch;
            mDirection.z = sinYaw * cosPitch;
            mDirection = glm::normalize(mDirection);

            mUp.x = cosYaw * cosPitch90;
            mUp.y = sinPitch90;
            mUp.z = sinYaw * cosPitch90;
            mUp = glm::normalize(mUp);

            const glm::vec3 &position = Camera::getPosition();
            Camera::lookAt(position, position + mDirection, glm::vec3(0.0f, 1.0, 0.0f));

            mFrustum.update(mTransformComponent.getPosition(), mDirection, mUp);

            mDirty = false;
            mTransformComponent.setDirty(false);
        }
    }
}
