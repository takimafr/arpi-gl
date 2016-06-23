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


#ifndef _DMA_SCENEMANAGER_HPP_
#define _DMA_SCENEMANAGER_HPP_

#include "utils/ExceptionHandler.hpp"
#include "engine/Entity.hpp"
#include "rendering/SkyBox.hpp"
#include "rendering/RenderingEngine.hpp"
#include "animation/AnimationSystem.hpp"
#include "resource/ResourceManager.hpp"
#include <rendering/Selectable.hpp>
#include "glm/glm.hpp"

#include <set>

namespace dma {

    class Scene {

        friend class Engine;

    public:
        Scene(std::shared_ptr<ResourceManager> resourceManager,
              std::shared_ptr<AnimationSystem> animationSystem,
              std::shared_ptr<RenderingEngine> renderingEngine);

    public:
        virtual ~Scene();

        void unload();

        void refresh();
        void wipe();

        /**
         * Gets the scene camera
         */
        inline Camera& getCamera() { return *mCamera; }

        /**
         * Sets the scene camera
         */
        void setCamera(std::shared_ptr<Camera> camera);

        void setSkyBox(const std::string &sid);
        void setSkyBoxEnabled(bool enabled);


        void setLightSource(const Light& light);

        std::shared_ptr<Entity> pick(int screenX, int screenY);

        glm::vec3 castRay(int screenX, int screenY);

        float distanceFromCamera(std::shared_ptr<Entity> entity);

        /**
         * 1. Checks if an origin shift is necessary. (TODO)
         *
         * 2. Determines whether an entity is potentially displayable
         *    and if so supplies it's rendering packages to the rendering engine. (TODO)
         */
        void step(float dt);

        inline std::set<std::shared_ptr<Entity>>& getEntities() { return mEntities; }

        /**
         * Adds an Entity to the scene.
         * @return true if entity added, false if it already belongs to the Scene.
         */
        bool addEntity(std::shared_ptr<Entity> entity);

        /**
         * remove the given entity from the scene.
         * You should also consider to call EntityFactory.deleteEntity().
         *
         * @param Entity&
         *              the entity to remove.
         * @return true if successfully removed.
         */
        bool removeEntity(std::shared_ptr<Entity> entity);

        /**
         * @param Entity&
         *              the entity to search.
         * @return true if the entity belongs to the scene.
         */
        bool hasEntity(std::shared_ptr<Entity> entity) ;

    private:
        /* ***
         * ATTRIBUTES
         */
        std::shared_ptr<Camera> mCamera;
        std::shared_ptr<SkyBox> mSkyBox;
        std::shared_ptr<ResourceManager> mResourceManager;
        std::shared_ptr<AnimationSystem> mAnimationSystem;
        std::shared_ptr<RenderingEngine> mRenderingEngine;
        std::set<std::shared_ptr<Entity>> mEntities;
        std::string mCurrentSkyboxSid = "default";
        bool mSkyboxEnabled = false;
    };
}

#endif //_DMA_SCENEMANAGER_HPP_
