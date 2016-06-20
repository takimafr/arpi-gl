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



#include <algorithm>

#include "engine/Scene.hpp"
#include "glm/gtx/string_cast.hpp"

constexpr char TAG[] = "Scene";

namespace dma {

    //----------------------------------------------------------------------
    Scene::Scene(ResourceManager* resourceManager,
                 AnimationSystem* animationSystem,
                 RenderingEngine* renderingEngine) :
            mCamera(nullptr),
            mSkyBox(nullptr),
            mResourceManager(resourceManager),
            mAnimationSystem(animationSystem),
            mRenderingEngine(renderingEngine)
    {
        //set default light source
        Light light(glm::vec3(-100.0f, 500.0f, 100.0f),
                    glm::vec3(1.0f),
                    glm::vec3(1.0),
                    glm::vec3(1.0));
        mRenderingEngine->setLight(light);
    }


    /*============================== PUBLIC ==============================*/

    //----------------------------------------------------------------------
    Scene::~Scene() {
        delete mSkyBox;
    }


    //----------------------------------------------------------------------
    void Scene::unload() {
        Log::trace(TAG, "Unloading Scene...");
        mEntities.clear();
        Log::trace(TAG, "Scene unloaded");
    }


    //----------------------------------------------------------------------
    void Scene::refresh() {
        Log::trace(TAG, "Refreshing Scene...");
        if (mSkyBox != nullptr) {
            mSkyBox->refresh();
        }
        Log::trace(TAG, "Scene refreshed");
    }


    //----------------------------------------------------------------------
    void Scene::setCamera(std::shared_ptr<Camera> camera) {
        mCamera = camera;
        mRenderingEngine->setVP(camera->getView(), camera->getProjection());
    }


    //----------------------------------------------------------------------
    Status Scene::setSkyBox(const std::string &sid) {
        Status result = STATUS_OK;

        mCurrentSkyboxSid = sid;

        if (mSkyboxEnabled) {
            std::shared_ptr<CubeMap> cubeMap = mResourceManager->acquireCubeMap("skybox/" + sid, &result);
            if (result != STATUS_OK) {
                Log::warn(TAG, "Cannot use skybox %s, cubemap doesn't exist", sid.c_str());
                return result;
            }
            if (mSkyBox == nullptr) {
                mSkyBox = new SkyBox();
                mSkyBox->init(cubeMap, mResourceManager->acquireShaderProgram("skybox", &result));
            }
            mSkyBox->setCubeMap(cubeMap);
            mRenderingEngine->setSkyBox(mSkyBox);
        }
        return result;
    }

    //----------------------------------------------------------------------
    Status Scene::setSkyBoxEnabled(bool enabled) {
        bool shouldEnable = enabled && !mSkyboxEnabled;
        bool shouldDisable = mSkyboxEnabled && !enabled;
        mSkyboxEnabled = enabled;

        if (shouldEnable) {
            return setSkyBox(mCurrentSkyboxSid);
        } else if (shouldDisable) {
            // don't forger to get the rendering engine to stop drawing the deleted skybox.
            mRenderingEngine->setSkyBox(nullptr);

            delete mSkyBox;
            mSkyBox = nullptr;


        }
        return STATUS_OK;
    }


    //----------------------------------------------------------------------
    void Scene::setLightSource(const Light& light) {
        mRenderingEngine->setLight(light);
    }


    //----------------------------------------------------------------------
    std::shared_ptr<Entity> Scene::pick(int screenX, int screenY) {

        //TODO implement
//        glm::vec3 ray = castRay(screenX, screenY);
//
//        std::vector<std::shared_ptr<Entity>> selectables;
//        for (auto entity : mEntities) {
//            if (std::shared_ptr<Selectable> s = std::dynamic_pointer_cast<Selectable>(entity)) {
//                if (s->intersects(ray, mCamera->getPosition())) {
//                    selectables.push_back(entity);
//                }
//            }
//        }
//
//        if (selectables.empty()) {
//            return nullptr;
//        }
//
//        std::shared_ptr<Entity> result = selectables[0];
//        float distanceMin = glm::length<float>(result->getPosition() - mCamera->getPosition());
//        for (U32 i = 1; i < selectables.size(); ++i) {
//            float d = glm::length<float>(selectables[i]->getPosition() - mCamera->getPosition());
//            if (d < distanceMin) {
//                result = selectables[i];
//                distanceMin = d;
//            }
//        }
//        return result;
        return nullptr;
    }


    //----------------------------------------------------------------------
    glm::vec3 Scene::castRay(int screenX, int screenY) {
        float x = (2.0f * screenX) / mRenderingEngine->getViewportWidth() - 1.0f;
        float y = 1.0f - (2.0f * screenY) / mRenderingEngine->getViewportHeight();
        float z = 1.0f;
        glm::vec3 ray_nds = glm::vec3(x, y, z);

        glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);

        glm::vec4 ray_eye = glm::inverse(mCamera->getProjection()) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

        glm::vec3 ray_wor = glm::vec3(glm::inverse(mCamera->getView()) * ray_eye);
        // don't forget to normalise the vector at some point
        ray_wor = glm::normalize(ray_wor);

        //Log::debug(TAG, "%f %f %f", ray_wor.x, ray_wor.y, ray_wor.z);
        return ray_wor;
    }


    //----------------------------------------------------------------------
    float Scene::distanceFromCamera(std::shared_ptr<Entity> entity) {
        glm::vec3 ce = entity->getPosition() - mCamera->getPosition(); // camera-entity vector
        return glm::length<float>(ce);
    }


    //----------------------------------------------------------------------
    void Scene::step(float dt) {
        assert(mCamera != nullptr && "Camera not set before calling Scene#step");
        mCamera->update(dt);
        for (auto e : mEntities) {
            assert(e != nullptr);
            e->update(dt);
            if (e->isRenderable()) {
                const RenderingComponent *rc = e->getRenderingComponent();
                assert(rc != nullptr);

                // frustum culling
                const BoundingSphere& sphere = rc->getMesh()->getBoundingSphere();
                if (mCamera->containsSphere(glm::vec3(*e->getM() * glm::vec4(sphere.getCenter(), 1.0f)), sphere.getRadius())) {
                    // Computes the distance to the camera
                    float distance = distanceFromCamera(e);
                    mRenderingEngine->subscribe(rc, distance);
                }
            }
        }
    }


    //----------------------------------------------------------------------
    bool Scene::addEntity(std::shared_ptr<Entity> entity) {
        return mEntities.insert(entity).second;
    }


    //----------------------------------------------------------------------
    bool Scene::removeEntity(std::shared_ptr<Entity> entity) {
        if (mEntities.erase(entity) == 0) {
            Log::warn(TAG, "Cannot remove entity since it doesn't belong to the scene");
            assert(!"Cannot remove entity since it doesn't belong to the scene");
            return false;
        }
        return true;
    }

    //----------------------------------------------------------------------
    bool Scene::hasEntity(std::shared_ptr<Entity> entity) {
        return mEntities.find(entity) != mEntities.end();
    }

    //----------------------------------------------------------------------
    void Scene::wipe() {
        if (mSkyBox != nullptr) {
            mSkyBox->wipe();
        }
    }
}
