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


#ifndef _DMA_TRANSFORMCOMPONENT_HPP_
#define _DMA_TRANSFORMCOMPONENT_HPP_

#define GLM_FORCE_CXX98
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace dma {

        class TransformComponent {

        public:
            TransformComponent();
            virtual ~TransformComponent();

            inline const glm::mat4& getM() const { return mM; }

            inline const glm::vec3& getPosition() { return mPosition; }
            void setPosition(const glm::vec3& position);

            void translate(const glm::vec3& translation);

            inline const glm::mat4 getOrientationMatrix() {
                return glm::mat4_cast(mOrientation);
            }
            inline const glm::quat& getOrientationQuat() {
                return mOrientation;
            }
            void setOrientation(const glm::quat& rotationQuat);

            void setOrientation(const glm::mat4 &rotationMatrix) {
                setOrientation(glm::quat_cast(rotationMatrix));
            }


            /**
             * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
             * @param angle the rotation angle in degrees
             * @param axis the rotation axis, must be normalized
             */
            void rotate(const float angle, const glm::vec3& axis);

            /**
             * Increases the pitch by angle degrees
             * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
             * @param angle the pitch angle to add
             */
            void pitch(const float angle);

            /**
            * Increases the yaw by angle degrees
            * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
            * @param angle the yaw angle to add
            */
            void yaw(const float angle);
            /**
             * Increases the roll by angle degrees
             * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
             * @param angle the roll angle to add
             */
            void roll(const float angle);


            void setScale(const glm::vec3& scale);

            /**
             * Updates the M matrix:
             * => T * R * S if reverse is false
             * => S * R * T otherwise
             */
            void update(const bool reverse = false);

            inline bool isDirty() { return mDirty; }
            inline void setDirty(bool dirty) { mDirty = dirty; }

        private:
            TransformComponent(const TransformComponent&) = delete;
            void operator=(const TransformComponent&) = delete;

            //FIELDS
            glm::vec3 mPosition;
            glm::quat mOrientation;
            glm::vec3 mScale;
            glm::mat4 mM;
            /** indicates if we need to recalculate the matrix */
            bool mDirty;
        };
}

#endif //_DMA_TRANSFORMCOMPONENT_HPP_
