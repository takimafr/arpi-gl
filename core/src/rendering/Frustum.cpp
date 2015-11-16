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



#include <glm/gtc/matrix_transform.hpp>
#include "rendering/Frustum.hpp"
#include "utils/Log.hpp"

#define TAG "Frustum"

namespace dma {

    //-----------------------------------------------------------------------------
    Frustum::Frustum() {

    }


    //-----------------------------------------------------------------------------
    Frustum::~Frustum() {

    }


    //-----------------------------------------------------------------------------
    void Frustum::setPerspective(float fovy, float aspect, float zNear, float zFar) {
        mFovY = fovy;
        mAspectRatio = aspect;
        mNear = zNear;
        mFar = zFar;

        // compute width and height of the near and far plane sections
        float t = (float)tan(glm::radians(fovy * 0.5));
        nh = mNear * t;
        nw = nh * mAspectRatio;
        fh = mFar  * t;
        fw = fh * mAspectRatio;

        // update projection matrix
        mProjection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);

        //Log::debug(TAG, "nw=%f, nh=%f, fw=%f, fh=%f", nw, nh, fw, fh);
    }


    //-----------------------------------------------------------------------------
    void Frustum::update(const glm::vec3& p, const glm::vec3& d,  const glm::vec3& up) {
        glm::vec3 nc, fc, m; //points
        glm::vec3 right, a, normal; // direction vectors

        right = glm::cross(up, d);

        // compute the centers of the near and far planes
        nc = p + d * mNear;
        fc = p + d * mFar;

        // NEAR & FAR
        mPlanes[NEAR].setNormal(-d);
        mPlanes[NEAR].setPoint(nc);
        mPlanes[FAR].setNormal(d);
        mPlanes[FAR].setPoint(fc);

        // TOP
        m = nc + up * nh ;/// 2.0f;
        a = m - p;
        a = glm::normalize(a);
        normal = glm::cross(a, right);
        mPlanes[TOP].setNormal(normal);
        mPlanes[TOP].setPoint(m);

        // BOTTOM
        m = nc - up * nh ;/// 2.0f;
        a = m - p;
        a = glm::normalize(a);
        normal = glm::cross(right, a);
        mPlanes[BOTTOM].setNormal(normal);
        mPlanes[BOTTOM].setPoint(m);

        // LEFT
        m = nc - right * nw ;/// 2.0f;
        a = m - p;
        a = glm::normalize(a);
        normal = glm::cross(a, up);
        mPlanes[LEFT].setNormal(normal);
        mPlanes[LEFT].setPoint(m);

        // RIGHT
        m = nc + right * nw ;/// 2.0f;
        a = m - p;
        a = glm::normalize(a);
        normal = glm::cross(up, a);
        mPlanes[RIGHT].setNormal(normal);
        mPlanes[RIGHT].setPoint(m);

    }


    //-----------------------------------------------------------------------------
    bool Frustum::containsSphere(const glm::vec3& center, float radius) {
        float distance;

        for (int i = 0; i < 6; i++) {
            distance = mPlanes[i].distance(center);
            //Log::debug(TAG, "PLANE=%d   |||   distance = %f  |  radius = %f  |  center = %f %f %f", i, distance, radius, center.x, center.y, center.z);
            if (distance > radius) {
                return false;
            }
        }
        return true;
    }
}