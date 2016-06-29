/*
Copyright © 2015 by eBusiness Information
All rights reserved. This source code or any portion thereof
may not be reproduced or used in any manner whatsoever
without the express written permission of eBusiness Information.
*/

#include <cassert>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include <string>
#include <rendering/FlyThroughCamera.hpp>
#include <rapidjson/document.h>
#include <resource/TrackFactory.hpp>
#include <utils/GeoUtils.hpp>
#include <animation/PathAnimation.hpp>
#include <animation/GeoPathAnimation.hpp>

#include "utils/ObjReader.hpp"
#include "engine/geo/GeoEngine.hpp"
#include "engine/geo/Poi.hpp"


using namespace dma;
using namespace dma::geo;

#define TAG "Main"

/* ***
 * GL CONSTANT PARAMETERS
 */
#define WIDTH 800
#define HEIGHT 600
#define WINDOW_TITLE "ARPIGL"

/* ***
 * ATTRIBUTES
 */
GLFWwindow* mWindow;
int mWidth, mHeight;
GeoEngine mGeoEngine("assets-test/arpigl");

bool keys[1024];

bool mRMBToggled = false;

double lastX = 0, lastY = 0;
float mSensibility = 0.3f;
float mCameraSpeed = 0.7f;

std::shared_ptr<FlyThroughCamera> mFlyThroughCamera = std::make_shared<FlyThroughCamera>();

std::vector<LatLngAlt> path;

/* ***
 * PRIVATE FUNCTION DEFS
 */
void mainLoop();


void mHandleEvents() {

    static bool skybox_enabled = false;
    //////////////////////////////////////////////////////////////
    // KEYBOARD
    if (keys[GLFW_KEY_W]) mFlyThroughCamera->forward(mCameraSpeed);
    if (keys[GLFW_KEY_S]) mFlyThroughCamera->backward(mCameraSpeed);
    if (keys[GLFW_KEY_A]) mFlyThroughCamera->strafeLeft(mCameraSpeed);
    if (keys[GLFW_KEY_D]) mFlyThroughCamera->strafeRight(mCameraSpeed);

    if (keys[GLFW_KEY_R]) {
        mGeoEngine.wipe();
        mGeoEngine.refresh();
        keys[GLFW_KEY_R] = false;
    }
    if (keys[GLFW_KEY_T]) {
        mGeoEngine.reload();
        keys[GLFW_KEY_T] = false;
    }
    if (keys[GLFW_KEY_Y]) {
        skybox_enabled = !skybox_enabled;
        mGeoEngine.setSkyBox("FullMoon");
        mGeoEngine.setSkyBoxEnabled(skybox_enabled);
        keys[GLFW_KEY_Y] = false;
    }
    if (keys[GLFW_KEY_U]) {
        skybox_enabled = !skybox_enabled;
        mGeoEngine.setSkyBoxEnabled(skybox_enabled);
        mGeoEngine.setSkyBox("SunSet");
        keys[GLFW_KEY_U] = false;
    }
    if (keys[GLFW_KEY_J]) {
        static int i = 0;
        static float speed = 50.0f;
        glm::vec3 a = mGeoEngine.getGeoSceneManager().mapPosition(path[i]);
        glm::vec3 b = mFlyThroughCamera->getPosition();
        float length = glm::length<float>(b - a);
        mGeoEngine.getGeoSceneManager().placeCamera(path[i], length / speed, TranslationAnimation::Function::LINEAR);
        i = (i + 1) % path.size();
        keys[GLFW_KEY_J] = false;
    }
    if (keys[GLFW_KEY_K]) {
        mGeoEngine.getGeoSceneManager().placeCamera(LatLngAlt(48.869252, 2.302800, 25.0f), 0.9f, TranslationAnimation::Function::EASE);
        keys[GLFW_KEY_K] = false;
//        mGeoEngine.getGeoSceneManager().placeCamera(LatLngAlt(81.221595, 2.0371413, 5.0f), 0.9f, TranslationAnimation::Function::EASE);
    }

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
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double screenX, screenY;
        glfwGetCursorPos(mWindow, &screenX, &screenY);
        mGeoEngine.getGeoSceneManager().pick((int) screenX, (int) screenY);
    }
}


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    mFlyThroughCamera->zoom((float) (yoffset * 0.1f));
}


inline bool mInitGlfwContext() {
    //init GLFW
    Log::info(TAG, "Initializing GLFW...");
    if (!glfwInit()) {
        Log::error(TAG, "Failed to initialize GLFW");
        return false;
    }

    //anti aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    Log::info(TAG, "Creating window...");
    mWindow = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!mWindow) {
        Log::error(TAG, "Failed to create window");
        glfwTerminate();
        return false;
    }
    Log::info(TAG, "Window created");

    glfwMakeContextCurrent(mWindow);

    glfwSwapInterval(1);

    glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);

    return true;
}


void windowSizeCallback(GLFWwindow* window, int width, int height) {
    mWidth = width;
    mHeight = height;
    // set viewport size.
    mGeoEngine.setSurfaceSize(mWidth, mHeight);
}



int main(int argc, char** argv) {

    // init glfw
    if (!mInitGlfwContext()) {
        Log::error(TAG, "error while initializing gl context. Abording...");
        assert(false);
        return -1;
    }

    // init engine
    if (!mGeoEngine.init()) {
        Log::error(TAG, "error while initializing engine...");
        return -1;
    }

    path.push_back(LatLngAlt(48.871069 , 2.303543, 15.0));
    path.push_back(LatLngAlt(48.870690 , 2.303191, 15.0));
    path.push_back(LatLngAlt(48.870307 , 2.302872, 15.0));
    path.push_back(LatLngAlt(48.869945 , 2.302545, 15.0));
    path.push_back(LatLngAlt(48.869571 , 2.302226, 15.0));
    path.push_back(LatLngAlt(48.869250 , 2.302792, 15.0));
    path.push_back(LatLngAlt(48.869504 , 2.303315, 15.0));
    path.push_back(LatLngAlt(48.869871 , 2.304026, 15.0));
    path.push_back(LatLngAlt(48.870169 , 2.304634, 15.0));
    path.push_back(LatLngAlt(48.870515 , 2.305281, 15.0));
    path.push_back(LatLngAlt(48.870706 , 2.304672, 15.0));
    path.push_back(LatLngAlt(48.870926 , 2.303964, 15.0));

    mGeoEngine.getGeoSceneManager().setTileNamespace("light");

    // Replace the default camera with a FlyThrough camera
    mGeoEngine.getGeoSceneManager().getScene().setCamera(mFlyThroughCamera);
    mGeoEngine.getGeoSceneManager().placeCamera(LatLngAlt(48.8708735, 2.3036656, 5.0));
//    mGeoEngine.getGeoSceneManager().placeCamera(LatLngAlt(48.870515, 2.305284, 5.0));

//    std::vector<glm::vec3> path2;
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.871069 , 2.303543, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.870690 , 2.303191, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.870307 , 2.302872, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.869945 , 2.302545, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.869571 , 2.302226, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.869250 , 2.302792, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.869504 , 2.303315, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.869871 , 2.304026, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.870169 , 2.304634, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.870515 , 2.305281, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.870706 , 2.304672, 15.0)));
//    path2.push_back(mGeoEngine.getGeoSceneManager().mapPosition(LatLngAlt(48.870926 , 2.303964, 15.0)));
//    mFlyThroughCamera->getAnimationComponent().add(std::make_shared<PathAnimation>(mFlyThroughCamera->getTransformComponent(), path2, 50.0f, true));
//    mFlyThroughCamera->getAnimationComponent().add(std::make_shared<GeoPathAnimation>(mFlyThroughCamera->getTransformComponent(), path, 25.0f, true, mGeoEngine.getGeoSceneManager()));

    mGeoEngine.setSkyBoxEnabled(true);

    std::shared_ptr<Poi> poi1 = mGeoEngine.getPoiFactory().builder()
            .sid("poi1")
            .shape("defibrillator")
                    //.icon("transilien/pins-agence_transilien")
            .animation(true)
            .color(Color(1.0f, 0.5f, 0.5f))
            .build();
    poi1->setCoords(LatLngAlt(48.8708735, 2.3036656, 6.0));
    mGeoEngine.getGeoSceneManager().addGeoEntity(poi1->getSid(), poi1);

    std::shared_ptr<Poi> poi2 = mGeoEngine.getPoiFactory().builder()
            .sid("poi2")
            .shape("pyramid")
            .color(Color(0.4f, 0.2f, 0.7f))
            .build();
    poi2->setCoords(LatLngAlt(45.784448, 4.854478, 6.0));
    mGeoEngine.getGeoSceneManager().addGeoEntity(poi2->getSid(), poi2);

    std::shared_ptr<Poi> poi3 = mGeoEngine.getPoiFactory().builder()
            .sid("poi3")
            .shape("note")
            .icon("b20")
            .color(Color(0.7f, 0.4f, 0.2f))
            .build();
    poi3->setCoords(LatLngAlt(45.784648, 4.854978, 6.0));
    mGeoEngine.getGeoSceneManager().addGeoEntity(poi3->getSid(), poi3);

    // set viewport size.
    mGeoEngine.setSurfaceSize(mWidth, mHeight);

    /////////////////////////////////////////////////////////
    // Register callbacks
    glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetMouseButtonCallback(mWindow, mouseCallback);
    glfwSetScrollCallback(mWindow, scrollCallback);




    /////////////////////////////////////////////////////////
    // Start main loop
    Log::info(TAG, "Starting main loop...");
    mainLoop();
    Log::info(TAG, "Main loop finished");

    //mEngine.removePoi("poi3");

    mGeoEngine.unload();


    Log::info(TAG, "Terminating...");
    glfwDestroyWindow(mWindow);
    glfwTerminate();

    return 0;

}

void mainLoop() {
    while (!glfwWindowShouldClose(mWindow)) {
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        mHandleEvents();
        mGeoEngine.step();
    }
}
