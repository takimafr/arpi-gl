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



#include "glm/gtc/matrix_transform.hpp"
#include "engine/TransformComponent.hpp"

namespace dma {


    //------------------------------------------------------
    TransformComponent::TransformComponent() :
                    mPosition(glm::vec3(0.0f)),
                    mOrientation(glm::quat()), //identity quaternion
                    mScale(glm::vec3(1.0f)),
                    mM(glm::mat4(1.0f)),
                    mDirty(true)
    {}


    //------------------------------------------------------
    TransformComponent::~TransformComponent() {

    }


    //------------------------------------------------------
    void TransformComponent::setPosition(const glm::vec3& position) {
        mPosition = position;
        mDirty = true;
    }


    //------------------------------------------------------
    void TransformComponent::translate(const glm::vec3& translation) {
        mPosition += translation;
        mDirty = true;
    }


    //------------------------------------------------------
    void TransformComponent::setOrientation(const glm::quat& rotationQuat) {
        mOrientation = rotationQuat;
        mDirty = true;
    }


    //------------------------------------------------------
    void TransformComponent::rotate(const float angle, const glm::vec3 &axis) {
        glm::quat q = glm::angleAxis(glm::radians(angle), axis);
        mOrientation = q * mOrientation;
        mDirty = true;
    }


    //------------------------------------------------------
    void TransformComponent::pitch(const float angle) {
        rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
    }


    //------------------------------------------------------
    void TransformComponent::yaw(const float angle) {
        rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    }


    //------------------------------------------------------
    void TransformComponent::roll(const float angle) {
        rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
    }


    //------------------------------------------------------
    void TransformComponent::setScale(const glm::vec3 &scale) {
        mScale = scale;
        mDirty = true;
    }


    //------------------------------------------------------
    void TransformComponent::update(const bool reverse) {
        if (mDirty) {
            if (reverse) {
                mM = glm::scale(glm::mat4(1.0f), mScale) * glm::mat4_cast(mOrientation) * glm::translate(glm::mat4(1.0f), mPosition);
            } else {
                mM = glm::translate(glm::mat4(1.0f), mPosition) * glm::mat4_cast(mOrientation) * glm::scale(glm::mat4(1.0f), mScale);
            }
            mDirty = false;
        }
    }
}
