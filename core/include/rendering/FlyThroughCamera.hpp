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


#ifndef _DMA_FLYTHROUGHCAMERA_HPP_
#define _DMA_FLYTHROUGHCAMERA_HPP_

#include "rendering/Camera.hpp"

namespace dma {

    class FlyThroughCamera : public Camera {

    public:
        /* ---------------- CONSTRUCTORS ----------------*/
        FlyThroughCamera();
        virtual ~FlyThroughCamera();

        FlyThroughCamera(const FlyThroughCamera&) = delete;
        FlyThroughCamera& operator=(const FlyThroughCamera&) = delete;


        void forward(const float offset);
        void backward(const float offset);
        void strafeRight(const float offset);
        void strafeLeft(const float offset);
        void pitch(const float offset);
        void yaw(const float offset);

        virtual void update(float dt) override;

    private:
        /* ---------------- FIELDS ----------------*/
        float mPitch;
        float mYaw;
        glm::vec3 mDirection;
        glm::vec3 mUp;
    };
}


#endif //_DMA_FLYTHROUGHCAMERA_HPP_
