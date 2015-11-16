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


#ifndef ARPIGL_FRUSTUM_HPP
#define ARPIGL_FRUSTUM_HPP

#include "rendering/Plane.hpp"
#include "rendering/BoundingSphere.hpp"

namespace dma {

    class Frustum {

        enum { NEAR = 0, FAR = 1, TOP = 2, BOTTOM = 3, LEFT = 4, RIGHT = 5};

        Plane mPlanes[6];
        float mNear, mFar, mAspectRatio, mFovY;
        float nw, nh, fw, fh;
        glm::mat4 mProjection;

    public:

        Frustum();
        virtual ~Frustum();

        inline float getNear() const {
            return mNear;
        }
        inline float getFar() const {
            return mFar;
        }
        inline float getAspectRatio() const {
            return mAspectRatio;
        }
        inline float getFovY() const {
            return mFovY;
        }
        inline const glm::mat4& getProjection() const { return mProjection; }

        void setPerspective(float fovy, float aspect,
                            float zNear,float zFar);

        inline void setAspectRatio(float aspect) {
            setPerspective(mFovY, aspect, mNear, mFar);
        }

        inline void setFovY(float fovy) {
            setPerspective(fovy, mAspectRatio, mNear, mFar);
        }

        /**
         * @note p and up must be unit vectors
         */
        void update(const glm::vec3&p, const glm::vec3&d,  const glm::vec3& up);

        bool containsSphere(const glm::vec3& center, float radius);

    };
}


#endif //ARPIGL_FRUSTUM_HPP
