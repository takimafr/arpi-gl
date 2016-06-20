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



#include "glm/gtc/matrix_transform.hpp"
#include "engine/Entity.hpp"

namespace dma {



    Entity::Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const glm::vec3& pos) :
            mTransformComponent(new TransformComponent()),
            mRenderingComponent(new RenderingComponent(mTransformComponent->getM(), mesh, material)),
            mAnimationComponent(NULL)
    {
        mTransformComponent->setPosition(glm::vec3(pos));
    }



    Entity::Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, float x, float y, float z) :
            Entity(mesh, material, glm::vec3(x, y, z)) {
    }



    Entity::Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) :
            Entity(mesh, material, 0.0f, 0.0f, 0.0f) {
    }



    Entity::~Entity() {
        delete mAnimationComponent;
        delete mRenderingComponent;
        delete mTransformComponent;
    }



    void Entity::setMesh(std::shared_ptr<Mesh> mesh) {
        mRenderingComponent->setMesh(mesh);
    }



    void Entity::setMaterial(std::shared_ptr<Material> material) {
        mRenderingComponent->setMaterial(material);
    }



    void Entity::update(float dt) {
        assert(mTransformComponent != NULL);
        mTransformComponent->update();
        if (mAnimationComponent != nullptr) {
            mAnimationComponent->update(dt);
        }
    }



    void Entity::addAnimationComponent() {
        delete mAnimationComponent;
        mAnimationComponent = new AnimationComponent(*mTransformComponent);
    }

} /* namespace dma */
