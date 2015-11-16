/*
Copyright © 2015 by eBusiness Information
All rights reserved. This source code or any portion thereof
may not be reproduced or used in any manner whatsoever
without the express written permission of eBusiness Information.
*/


// OpenGL
#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

//std
#include <string>
#include <cassert>

// cute
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

// dma
#include "utils/Log.hpp"
#include "utils/ObjReader.hpp"
#include "utils/MaterialReader.hpp"
#include "rendering/FlyThroughCameraHolder.hpp"
#include "GeoEngineAsync.hpp"
#include "engine/geo/PoiFactory.hpp"
#include "glm/glm.hpp"

#include "cute_suite.h"

extern cute::suite make_suite_dmaTribeNativeTest();
#define LOG_TAG "TEST"

struct TestResource {
    static const std::string resFolder;

    struct Mesh {
        static const std::string cube;

    };

    struct Material {
        static const std::string cube;
    };

    struct Scene {
        static const std::string inno;
        static const std::string invalid;

    };

};

struct TestConfig {
    struct Camera {
           static constexpr double lat = 0;
           static constexpr double lon = 0;
           static constexpr double alt = 70;

           static constexpr double pitch = -90;
           static constexpr double yaw = 0;
           static constexpr double roll = 0;

       };
};

enum CallbackTimeout {
    NONE = 0,
    SHORT = 250,
    MEDIUM = 500,
    LONG = 750,
    EXTRA = 1500,
    DEFAULT = NONE
};

/* ***
 * GL CONSTANT PARAMETERS
 */
#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "Simple example"

/* ***
 * DEBUG & TEST PARAMS
 */

/* ***
 * PROTOTYPE DEFS
 */
void mouseCallback(GLFWwindow* window, int button, int action, int mods);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

/* ***
 * ATTRIBUTES
 */
GLFWwindow* mWindow;
int mWidth, mHeight;
dma::geo::GeoEngineProxy*mGeoEngine;
dma::FlyThroughCameraHolder* mFlyThroughCamera;
dma::geo::GeoCameraHolder* mGeoCamera;
bool keys[1024];

bool mRMBToggled = false;

double lastX = 0, lastY = 0;
float mSensibility = 0.3f;
float mCameraSpeed = 0.7f;
bool mTestPassed = true;
cute::suite mCuteSuite;




inline bool initGlfwContext() {

    //init GLFW
    dma::Log::trace(LOG_TAG, "Initializing GLFW...");
    if (!glfwInit()) {
        dma::Log::error(LOG_TAG, "Failed to initialize GLFW");
        return false;
    }

    dma::Log::trace(LOG_TAG, "Creating window...");
    mWindow = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!mWindow) {
        dma::Log::error(LOG_TAG, "Failed to create window");
        glfwTerminate();
        return false;
    }
    dma::Log::trace(LOG_TAG, "Window created");

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

    glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);

    glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetMouseButtonCallback(mWindow, mouseCallback);
    glfwSetScrollCallback(mWindow, scrollCallback);

    return true;
}

inline void destroyGlContext() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}


void handleEvents() {
    //////////////////////////////////////////////////////////////
    // KEYBOARD
    if (keys[GLFW_KEY_W]) mFlyThroughCamera->forward(mCameraSpeed);
    if (keys[GLFW_KEY_S]) mFlyThroughCamera->backward(mCameraSpeed);
    if (keys[GLFW_KEY_A]) mFlyThroughCamera->strafeLeft(mCameraSpeed);
    if (keys[GLFW_KEY_D]) mFlyThroughCamera->strafeRight(mCameraSpeed);

    //////////////////////////////////////////////////////////////
    // MOUSE
    if (mRMBToggled) {
        double x, y, offsetX, offsetY;
        glfwGetCursorPos(mWindow, &x, &y);
        offsetX = x - lastX;
        offsetY = lastY - y; //screen coordinates
        lastX = x;
        lastY = y;
        if (offsetX != 0) {
            mFlyThroughCamera->yaw((const float) offsetX * mSensibility);
        }
        if (offsetY != 0) {
            mFlyThroughCamera->pitch((const float) offsetY * mSensibility);
        }
    }
}


/////////////////////////////////////////////////////////////
//                      CALLBACKS                          //
/////////////////////////////////////////////////////////////

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {
        keys[key] = true;
    }
    if(action == GLFW_RELEASE) {
        keys[key] = false;
    }
    if(keys[GLFW_KEY_Y] || keys[GLFW_KEY_ENTER]) {
        glfwSetWindowShouldClose(mWindow, GL_TRUE);
        keys[key] = false;
        mTestPassed = true;
        return;
    }
    if(keys[GLFW_KEY_N]) {
        glfwSetWindowShouldClose(mWindow, GL_TRUE);
        keys[key] = false;
        mTestPassed = false;
        return;
    }
}


void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glfwGetCursorPos(mWindow, &lastX, &lastY);
        mRMBToggled = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        mRMBToggled = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    mFlyThroughCamera->zoom((float) (yoffset * 0.1f));
}

void windowSizeCallback(GLFWwindow* window, int width, int height) {
    mWidth = width;
    mHeight = height;
    // set viewport size.
    mGeoEngine->setSurfaceSize(mWidth, mHeight);
}

struct LoopCallback {
    virtual ~LoopCallback() {

    }
    virtual void operator()() = 0;
};

struct TimeoutCallback : LoopCallback {

    unsigned int mMaxFrames;
    unsigned int mCurrentFrame;

    TimeoutCallback(CallbackTimeout nbFrameAlive = DEFAULT) {
        mMaxFrames = nbFrameAlive;
        mCurrentFrame = 0;
    }

    virtual void operator()() {
        if(++mCurrentFrame == mMaxFrames) {
            glfwSetWindowShouldClose(mWindow, GL_TRUE);
        }
    }
};


void mainLoop(LoopCallback* callback = NULL) {

    while (!glfwWindowShouldClose(mWindow)) {
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        handleEvents();
        mGeoEngine->step();
        if(callback) {
            (*callback)();
        }
    }
}

#define onInit()                                                                 \
    if(!mTestPassed) return;                                                     \
    mGeoEngine = new dma::geo::GeoEngineProxy(TestResource::resFolder);             \
    mGeoEngine->setSkyBoxEnabled(false);                                            \
    mGeoCamera = &(mGeoEngine->getCamera());                                        \
    mFlyThroughCamera = new FlyThroughCameraHolder(mGeoCamera);                  \
    mFlyThroughCamera->pitch(TestConfig::Camera::pitch);                         \
    mGeoCamera->setPosition(TestConfig::Camera::lat,                             \
            TestConfig::Camera::lon,                                             \
            TestConfig::Camera::alt);                                            \
    bool res = !mGeoEngine->isInit();                                               \
    ASSERTM("is not init", res);                                                 \
    res = initGlfwContext();                                                     \
    ASSERTM("init openGL context", res);                                         \
    mGeoEngine->init();                                                             \
    ASSERTM("init engine", res);                                                 \
    res = mGeoEngine->isInit();                                                     \
    ASSERTM("is init", res);                                                     \
    mGeoEngine->setSurfaceSize(mWidth, mHeight);                                    \
    ASSERTM("set engine viewport", res);                                         \


#define onUnload()                                       \
    mEngine->unload();                                   \
    ASSERTM("is not init", !mEngine->isInit());          \


#define onDestroy()                                      \
    mGeoEngine->unload();                                   \
    mGeoEngine->unload();                                   \
    ASSERTM("is not init", !mEngine->isInit());          \
    destroyGlContext();                                  \
    delete mGeoEngine;                                      \
    ASSERTM("test failed", mTestPassed);                 \
    delete mFlyThroughCamera;                            \

