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



// Standard cpp
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <algorithm>
// Dma
#include "engine/Engine.hpp"
#include "engine/geo/Poi.hpp"

#define TAG "Engine"

namespace dma {
#ifndef FPS_PRINT_RATE
    constexpr float Engine::FPS_PRINT_RATE;
#endif

    /* ***
     * CONSTRUCTORS
     */
    //---------------------------------------------------------------------------------
    Engine::Engine(const std::string& rootDir) :
            mRootDir(rootDir),
            mIsInit(false) {
        Log::trace(TAG, "Creating Engine...");

        mRootDir = Utils::addTrailingSlash(mRootDir);
        mResourceManager = new ResourceManager(mRootDir);
        mGlobalTimer = new Timer();
        mRenderingEngine = new RenderingEngine(*mResourceManager);
        mAnimationSystem = new AnimationSystem();
        mScene = new Scene(mResourceManager, mAnimationSystem, mRenderingEngine);

        Log::trace(TAG, "resource dir: %s", mRootDir.c_str());
        assert(Utils::dirExists(mRootDir.c_str()));

        Log::trace(TAG, "-----Engine created-----\n");
    }

    //---------------------------------------------------------------------------------

    Engine::~Engine() {
        Log::trace(TAG, "Destroying Engine...");
        delete mScene;
        delete mAnimationSystem;
        delete mResourceManager;
        delete mGlobalTimer;
        delete mRenderingEngine;
        Log::trace(TAG, "-----Engine destroyed-----\n");
    }

    /* ***
     * PUBLIC METHODS
     */
    //---------------------------------------------------------------------------------
    void Engine::setSurfaceSize(U32 width, U32 height) {
        Log::trace(TAG, "Setting surface size...");

        //init already called ?
#ifdef DEBUG
        if (!mIsInit) {
            Log::warn(TAG, "Setting viewport size while engine not initialized...");
        }
#endif
        //check width & height param
        if (width < 1 || height < 1) {
            Log::error(TAG, "invalid size : (%d, %d)", width, height);
            throwError(TAG, ExceptionType::INVALID_VALUE, "Engine::setSurfaceSize : Invalid width or height");
        }
        mRenderingEngine->setViewport(width, height);
        mScene->getCamera().setAspectRatio(mRenderingEngine->getAspectRatio());

        Log::trace(TAG, "Surface size set");
    }


    //---------------------------------------------------------------------------------
    bool Engine::init() {
        // an openGL context must be opened for this operation.
        bool hasOglContext = GLUtils::hasGlContext();
        if(!hasOglContext) {
            Log::warn(TAG, "no openGL context has been created. 'init' may not work or may crash.");
        } else {
            GLUtils::printGlContext();
        }
        assert(hasOglContext);

        Log::trace(TAG, "Initializing Engine...");
        if (mIsInit) {
            Log::warn(TAG, "calling init twice.. This call will have no effect");
            return false;
        }

        mGlobalTimer->reset();
        if (mRenderingEngine->init() != STATUS_OK) {
            constexpr char error[] = "Error while initializing RenderingEngine";
            throwError(TAG, ExceptionType::UNKNOWN, error);
            assert(false);
            return false;
        }

        if (mResourceManager->init() != STATUS_OK) {
            constexpr char error[] = "Error while initializing ResourceManager";
            throwError(TAG, ExceptionType::UNKNOWN, error);
            assert(false);
            return false;
        }

        // engine is not initialized.
        mIsInit = true;

        Log::trace(TAG, "Engine initialized");
        return true;
    }

    //---------------------------------------------------------------------------------
    void Engine::refresh() {
        mAssertInit("Engine::refresh");

        mResourceManager->refresh();
        mScene->refresh();
        mRenderingEngine->init();
    }

    //---------------------------------------------------------------------------------
    void Engine::reload() {
        mAssertInit("Engine::reload");

        mResourceManager->reload();
        mScene->refresh(); //refresh skybox
        mRenderingEngine->init();
    }

    //---------------------------------------------------------------------------------
    void Engine::unload() {
        Log::trace(TAG, "Unloading Engine...");
        if(!mIsInit) {
            Log::trace(TAG, "Unloading engine that is not initialized. Will have no effect.");
            Log::trace(TAG, "Engine unloaded");
            return;
        }

        // if an openGL context is opened, try to clean up OGL resources.
        mScene->unload();
        mRenderingEngine->unload();
        mResourceManager->unload();
        mIsInit = false;
        Log::trace(TAG, "Engine unloaded");
    }


    //---------------------------------------------------------------------------------
    void Engine::wipe() {
        mAssertInit("Engine::wipe");

        Log::trace(TAG, "Wiping Engine...");
        mResourceManager->wipe();
        mScene->wipe(); //wipe skybox
        Log::trace(TAG, "Engine wiped");
    }


    //---------------------------------------------------------------------------------
    void Engine::step() {
        mAssertInit("Engine::step");
        mGlobalTimer->update();
#ifdef FPS_PRINT_RATE
        mUpdateFPS();
#endif
        mScene->step(mGlobalTimer->dt());
        mRenderingEngine->drawFrame();
    }


    //---------------------------------------------------------------------------------
    void Engine::addHUDElement(std::shared_ptr<HUDElement> hudElement) {
        mRenderingEngine->subscribeHUDElement(hudElement);
    }


    //---------------------------------------------------------------------------------
    void Engine::mUpdateFPS() {
        static float elapsedTime = 0.0f;
        static int frameCount = 0;

        frameCount += 1;
        elapsedTime += mGlobalTimer->dt();

        if (elapsedTime >= FPS_PRINT_RATE && FPS_PRINT_RATE > 0) {
            Log::info(TAG, "FPS = %f", (float)frameCount / elapsedTime);
            frameCount = 0;
            elapsedTime = 0.0f;
        }
    }


    //---------------------------------------------------------------------------------
#ifdef DEBUG
    void Engine::mAssertInit(const char* msg) const {

        std::string str = "Calling " + std::string(msg) + "() on a non-initialized engine.";
        if (!mIsInit) {
            Log::error(TAG, "%s", str.c_str());
            throwError(TAG, ExceptionType::INITIALIZATION, str);
            assert(false);
        }
    }
#endif


}//end of namespace

