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


/*
 * Engine.hpp
 *
 *  Created on: 12 mai 2015
 *      Author: excilys
 */

#ifndef _DMA_ENGINE_HPP_
#define _DMA_ENGINE_HPP_

// OpenGL
#include <utils/GLES2Logger.hpp>

//std
#include <string>
#include <list>

//Dma
#include "utils/ExceptionHandler.hpp"
#include "common/Timer.hpp"
#include "resource/ResourceManager.hpp"
#include "rendering/RenderingEngine.hpp"
#include "engine/Entity.hpp"
#include "rendering/Camera.hpp"
#include "Scene.hpp"
#include "animation/AnimationSystem.hpp"


namespace dma {

    /* **
     * @version 0.2.3
     */
    class Engine {
    public:

        /* ***
         * CONSTANTS
         */
#ifndef FPS_PRINT_RATE
        static constexpr float FPS_PRINT_RATE =  -2.0f;             // NEGATIVE TO DISABLE
#endif
        /* ***
         * CONSTRUCTORS
         */

        /**
         * Create a new Engine, by providing its root dir.
         * Root dir is needed to load resource files. An incorrect dir will cause the Engine fail to load.
         * @param std::string
         *              root directory path.
         */
        explicit Engine(const std::string& = "");
        virtual ~Engine();
        Engine(const Engine&) = delete;
        void operator=(const Engine&) = delete;


        virtual bool init();

        virtual void refresh();

        virtual void wipe();

        virtual void unload();

        virtual void reload();

        virtual void step();

        /* ***
         * OVERRIDDEN GETTERS
         */

        inline bool isInit() const {
            return mIsInit;
        }

        inline bool isAbleToDraw() const {
            return isInit() && GLUtils::hasGlContext();
        }

        virtual void addHUDElement(std::shared_ptr<HUDElement> hudElement);

        /* ***
         * PUBLIC SETTERS
         */

        virtual void setSurfaceSize(U32, U32);

        /* ***
         * PUBLIC GETTERS
         */


        inline ResourceManager& getResourceManager() const {
            return *mResourceManager;
        }


        inline AnimationSystem& getAnimationSystem() const {
            return *mAnimationSystem;
        }


        inline Timer& getGlobalTimer() const {
            return *mGlobalTimer;
        }


        inline Scene &getScene() const {
            return *mScene;
        }

    private:

        void mUpdateFPS();

        /* ***
         * ATTRIBUTES
         */
        /** root direcrtory path */
        std::string mRootDir;
        /** The global timer */
        Timer* mGlobalTimer;
        /** The resource manager */
        ResourceManager* mResourceManager;
        /** The rendering engine */
        RenderingEngine* mRenderingEngine;
        /** The Scene manager */
        Scene *mScene;
        /** The animation system */
        AnimationSystem* mAnimationSystem;
        /** is engine properly initialized. */
        bool mIsInit;

#ifdef DEBUG
        void mAssertInit(const char* msg) const;
#else
        inline void mAssertInit(const char* msg) const {}
#endif
    };

}
#endif /* _DMA_ENGINE_HPP_ */
