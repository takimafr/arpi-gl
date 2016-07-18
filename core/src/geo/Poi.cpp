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



#include "geo/Poi.hpp"

#define POI_PASS 1

namespace dma {

        constexpr char TAG[] = "Poi";


        Poi::Poi(const std::string& sid,
                 std::shared_ptr<Mesh> mesh,
                 std::shared_ptr<Material> material,
                 bool animated,
                 LatLng& geoSceneOrigin) :
                GeoEntity(mesh, material, geoSceneOrigin),
                mSID(sid),
                mAnimated(animated),
                mCurrentTranslationAnimation(nullptr),
                mCurrentRotationAnimation(nullptr)
        {
            if (mAnimated) {
                addAnimationComponent();
                animate();
            }
        }

        Poi::~Poi() {
        }

        void Poi::setCoords(const LatLngAlt &coords) {
            GeoEntity::setCoords(coords);
            animate();
        }

        void Poi::animate() {
            if (mAnimated) {
                assert(Entity::isAnimable());
                if (mCurrentTranslationAnimation != nullptr) {
                    mAnimationComponent->remove(mCurrentTranslationAnimation);
                    //delete mCurrentTranslationAnimation;
                }

                if (mCurrentRotationAnimation != nullptr) {
                    mAnimationComponent->remove(mCurrentRotationAnimation);
                    //delete mCurrentRotationAnimation;
                }

                mCurrentTranslationAnimation =
                        std::make_shared<TranslationAnimation>(*mTransformComponent,
                                                               mTransformComponent->getPosition(),
                                                               mTransformComponent->getPosition() + glm::vec3(0.0f, 2.0f, 0.0f),
                                                               6.0f, TranslationAnimation::Function::EASE, true, true);
                mAnimationComponent->add(mCurrentTranslationAnimation);

                mCurrentRotationAnimation =
                        std::make_shared<RotationAnimation>(*mTransformComponent,
                                                            8.0f, true, 360.0f,
                                                            glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
                mAnimationComponent->add(mCurrentRotationAnimation);
            }
        }

        void Poi::deanimate() {
            mAnimationComponent->remove(mCurrentTranslationAnimation);
            mCurrentTranslationAnimation = nullptr;
            mAnimationComponent->remove(mCurrentRotationAnimation);
            mCurrentRotationAnimation = nullptr;
        }

        void Poi::setColor(const Color &color) {
            getMaterial()->getPass(POI_PASS).setDiffuseColor(glm::vec3(color.r, color.g, color.b));
        }

        bool Poi::intersects(const glm::vec3 &ray, const glm::vec3& origin) {
            const BoundingSphere& boundingSphere = mRenderingComponent->getMesh()->getBoundingSphere();
            glm::vec3 oc = origin - glm::vec3(mTransformComponent->getM() * glm::vec4(boundingSphere.getCenter(), 1.0f));
            float b = glm::dot<float>(ray, oc);
            float c = glm::dot<float>(oc, oc) - boundingSphere.getRadius() * boundingSphere.getRadius();

            return b <= 0.0f && b * b - c >= 0.0f;
        }
}



