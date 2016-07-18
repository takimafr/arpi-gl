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


#ifndef _DMA_POI_HPP_
#define _DMA_POI_HPP_

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <utils/Color.hpp>
#include <rendering/Selectable.hpp>
#include <glm/gtc/type_ptr.hpp>     // make_mat4
#include "engine/Entity.hpp"
#include "animation/RotationAnimation.hpp"
#include "LatLngAlt.hpp"
#include "GeoEntity.hpp"

namespace dma {
    /**
     * Poi variation of an Entity. A poi, or Point of Interest, represents
     * a precise point located in space.
     * It is described by a shape, a material, and its position.
     *
     * @version 0.2.3
     */
    class Poi : public GeoEntity, public Selectable {

    public:
        Poi(const std::string& sid, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, bool animated, LatLng& geoSceneOrigin);
        Poi(const Poi &) = delete;
        void operator=(const Poi &) = delete;
        virtual ~Poi();


        /* ***
         * GETTERS
         */

        /** /!\ CAN BE EMPTY */
        inline const std::string& getSid() const { return mSID; }

        virtual void setCoords(const LatLngAlt &coords) override;

        void setColor(const Color& color);

        /**
         * Set the POI orientation.
         *
         * @param const float*
         *            the orientation matrix.
         */
        inline void setOrientation(const float orientationMat[16]) {
            const glm::mat4& rotationMatrix = glm::make_mat4(orientationMat);
            Entity::setOrientation(rotationMatrix);
        }

        void animate();
        void deanimate();

        inline std::shared_ptr<Material> getMaterial() {
            return mRenderingComponent->getRenderingPackages()[0]->getMaterial();
        }

        virtual bool intersects(const glm::vec3 &ray, const glm::vec3& origin);

        /* ***
         * ATTRIBUTES
         */

    protected:
        const std::string mSID;
        bool mAnimated;
        std::shared_ptr<TranslationAnimation> mCurrentTranslationAnimation;
        std::shared_ptr<RotationAnimation> mCurrentRotationAnimation;
    };
}

#endif //_DMA_POI_HPP_
