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
 * PoiFactory.h
 *
 *  Created on: 5 juin 2015
 *      Author: Nicolas THIERION.
 */

#ifndef _DMA_POIFACTORY_HPP_
#define _DMA_POIFACTORY_HPP_

#include <unordered_set>
#include <map>
#include <memory>
#include <string>
#include <resource/ResourceManager.hpp>

#include "utils/Color.hpp"
#include "utils/Log.hpp"

namespace dma {
    class EntityFactory;

    class Poi;
    class GeoEngine;

    /**
     * Factory class that has access to ResourceManager,
     * in order to construct Poi object, with a mesh & a material.
     * @version 0.2.3 - Changelog : removed exit codes in return values.
     */
    class PoiFactory {

    protected:

        class Builder {
        public:
            Builder(ResourceManager& resourceManager);
        public:
            Builder& sid(const std::string& sid);
            Builder& shape(const std::string& shape);
            Builder& icon(const std::string& icon);
            Builder& color(const Color& color);
            Builder &animation(bool animated);
            std::shared_ptr<Poi> build();
        protected:
            ResourceManager& mResourceManager;
            std::string mSid;
            std::string mShape;
            std::string mIcon;
            Color mColor;
            bool mAnimated;

        };

    public:
        PoiFactory(ResourceManager& resourceManager);
        virtual ~PoiFactory();
        PoiFactory(const PoiFactory&) = delete;
        PoiFactory& operator=(const PoiFactory&) = delete;

        Builder builder();

    private:
        ResourceManager& mResourceManager;
    };

} /* namespace dma */
#endif /* _DMA_POIFACTORY_HPP_ */
