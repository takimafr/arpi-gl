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


#ifndef _DMA_ENTITY_HPP_
#define _DMA_ENTITY_HPP_

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "rendering/RenderingComponent.hpp"
#include "resource/Mesh.hpp"
#include "resource/ShaderProgram.hpp"
#include "resource/Texture.hpp"
#include "common/Types.hpp"
#include "engine/TransformComponent.hpp"

#include <string>
#include "animation/AnimationComponent.hpp"

namespace dma {

        /**
         * An entity is a package of renderable elements, that can be put and drawn inside an Engine.
         * It is described by a shape, a material, and its position.
         */
        class Entity {

        public:

            void operator=(const Entity&) = delete;
            virtual ~Entity();

            /* ***
             * CONSTRUCTORS
             */

            /**
             * Create a new entity given its position.
             * The Entity will be renderable, and its shape is defined by the given Mesh parameter.
             * @param const Mesh& -
             *              shape of this entity.
             * @param const Material& -
             *              Material used by the mesh.
             * @param float -
             *              X position of this entity.
             * @param float -
             *              Y position of this entity.
             * @param float -
             *              Z position of this entity.
             */
            Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, float x, float y, float z);

            /**
             * Create a new entity given its position.
             * The Entity will be renderable, and its shape is defined by the given Mesh parameter.
             * @param const Mesh& -
             *              shape of this entity.
             * @param const Material& -
             *              Material used by the mesh.
             * @param const glm::vec3& -
             *              position of this entity.
             */
            Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const glm::vec3& pos);

            Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

            /* ***
             * PUBLIC METHODS
             */

            virtual inline void translate(const glm::vec3& translation) {
                mTransformComponent->translate(translation);
            }

            /**
             * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
             * @param float
             *               the rotation angle in degrees
             * @param float -
             *              X component of rotation axis.
             * @param float -
             *              Y component of rotation axis.
             * @param float -
             *              Z component of rotation axis.
             */
            virtual inline void rotate(float angle, float x, float y, float z) {
                rotate(angle, glm::vec3(x, y, z));
            }

            /**
             * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
             * @param float
             *               the rotation angle in degrees
             * @param const glm::vec3&
             *               the rotation axis, must be normalized
             */
            virtual inline void rotate(float angle, const glm::vec3& axis) {
                mTransformComponent->rotate(angle, axis);
            }

            /**
             * Increases the pitch by angle degrees
             * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
             * @param float
             *              the pitch angle to add, in degrees
             */
            virtual inline void pitch(float angle) {
                mTransformComponent->pitch(angle);
            }

            /**
             * Increases the yaw by angle degrees
             * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
             * @param float
             *              the yaw angle to add, in degrees
             */
            virtual inline void yaw(float angle) {
                mTransformComponent->yaw(angle);
            }

            /**
             * Increases the roll by angle degrees
             * @note Counter-clockwise rotation for angle > 0, and clockwise rotation otherwise.
             * @param float
             *              the roll angle to add, in degrees
             */
            virtual inline void roll(float angle) {
                mTransformComponent->roll(angle);
            }


            virtual inline void setScale(const glm::vec3& scale) {
                mTransformComponent->setScale(scale);
            }

            /* ***
             * GETTERS
             */

            /**
             * Get the current transform matrix (ie : rotation + translation) for
             * this entity.
             * @return the transformation matrix.
             */
            inline const glm::mat4* getM() const {
                return &mTransformComponent->getM();
            }

            /**
             * Get the RenderingComponent of this entity.
             * @return the rendering component.
             */
            inline const RenderingComponent* getRenderingComponent() const {
                assert(isRenderable());
                return mRenderingComponent;
            }

            /**
             * @return true if this entity is renderable (IE : 'setMesh()' & 'setMaterial()' has been both called).
             */
            inline bool isRenderable() const {
                return mRenderingComponent != nullptr;
            }

            inline bool isAnimable() const {
                return mAnimationComponent != nullptr;
            }

            virtual inline const glm::vec3& getPosition() const {
                return mTransformComponent->getPosition();
            }

            /* ***
             * SETTERS
             */

            /**
             * Set the position for this entity.
             * @param float -
             *              X position of this entity.
             * @param float -
             *              Y position of this entity.
             * @param float -
             *              Z position of this entity.
             */
            virtual inline void setPosition(float x, float y, float z) {
                setPosition(glm::vec3(x, y, z));
            }

            /**
             * Set the position for this entity.
             * @param glm::vec3 -
             *              position vector of this entity.
             */
            virtual inline void setPosition(const glm::vec3& position) {
                mTransformComponent->setPosition(position);
            }

            inline void setOrientation(const glm::mat4& rotationMatrix) {
                mTransformComponent->setOrientation(rotationMatrix);
            }

            /**
             * @param const Mesh& the mesh to be used by this entity.
             */
            void setMesh(std::shared_ptr<Mesh> mesh);

            /**
             * @param const Material& the material to be used by this mesh's entity.
             */
            void setMaterial(std::shared_ptr<Material>);


            TransformComponent& getTransformComponent() { return *mTransformComponent; }
            AnimationComponent* getAnimationComponent() { return mAnimationComponent; }

            void addAnimationComponent();
            /**
             * Updates entity components
             */
            virtual void update(float dt);

        protected:
            TransformComponent* mTransformComponent;
            RenderingComponent* mRenderingComponent;
            AnimationComponent* mAnimationComponent;
        };

} /* namespace dma */

#endif /* _DMA_ENTITY_HPP_ */
