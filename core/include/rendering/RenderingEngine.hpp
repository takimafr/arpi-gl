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


#ifndef _DMA_RENDERINGENGINE_HPP_
#define _DMA_RENDERINGENGINE_HPP_

#include "common/Types.hpp"
#include "rendering/Camera.hpp"
#include "rendering/RenderingPackage.hpp"
#include "rendering/RenderingComponent.hpp"
#include "rendering/SkyBox.hpp"
#include "rendering/Light.hpp"
#include "HUDSystem.hpp"

#include <list>
#include <queue>

namespace dma {

    class RenderingEngine {

    private:

        struct Entry {
            F32 distance;
            RenderingPackage* renderingPackage;
            bool operator<(const Entry & other) const {
                return (distance < other.distance);
            }
        };

    public:

        RenderingEngine(ResourceManager& resourceManager);
        RenderingEngine(const RenderingEngine&) = delete;
        void operator=(const RenderingEngine&) = delete;
        virtual ~RenderingEngine();


        Status init();

        /**
         * Free memory used by this resource manager.
         */
        void unload();

        void setViewport(U32 width, U32 height);

        inline U32 getViewportWidth() const { return mViewportWidth; }
        inline U32 getViewportHeight() const { return mViewportHeight; }

        inline void setSkyBox(std::shared_ptr<SkyBox> skyBox) { mSkyBox = skyBox; }

        inline void setLight(const Light& light) { mLight = light; }

        void subscribe(RenderingPackage* package, bool front2back, float distanceFromCamera);

        void subscribe(const RenderingComponent* component, float distanceFromCamera);

        void subscribeHUDElement(std::shared_ptr<HUDElement> hudElement);

        /**
         * Render the current frame.
         * setCamera must have been called with a valid camera before the first call to this method.
         */
        void drawFrame();

        /**
         * Sets the View and the Projection matrices
         */
        inline void setVP(const glm::mat4& V, const glm::mat4& P) {
            mV = &V;
            mP = &P;
        }


        inline F32 getAspectRatio() const { return mAspectRatio; }

    private:
        void mDraw(RenderingPackage* package, const glm::mat4& V, const glm::mat4& P);
        void mDrawSkyBox();

        HUDSystem mHUDSystem;
        std::shared_ptr<SkyBox> mSkyBox;
        Light mLight;
        const glm::mat4* mV;
        const glm::mat4* mP;
        U32 mViewportWidth;
        U32 mViewportHeight;
        F32 mAspectRatio;
        std::priority_queue<Entry> mFrontToBack;
        std::priority_queue<Entry> mBackToFront;
        std::priority_queue<Entry> mReverseTransparency; //TODO remove, used for buildings/tracks demo
        GLuint mAttribIndices[ShaderProgram::AttribSem::AS_size];
    };
}

#endif //_DMA_RENDERINGENGINE_HPP_
