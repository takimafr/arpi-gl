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


#ifndef _DMA_SCENEREADER_HPP
#define _DMA_SCENEREADER_HPP

#include "common/Types.hpp"
#include "rapidjson.h"
#include "document.h"
#include "glm/glm.hpp"
#include <string>

namespace dma {
    class GeoSceneReader {

    private:


        /* ***
         * CONSTANTS
         */
        static constexpr char  TAG[] = "GeoSceneReader";

        struct Keys {
            static constexpr char  ORIGIN[] = "origin";
            static constexpr char CAMERA[] = "camera";
            static constexpr char CAMERA_TYPE[] = "type";
            static constexpr char POIS[] = "POIs";
            static constexpr char SID[] = "sid";
            static constexpr char LAT[] = "lat";
            static constexpr char LON[] = "lon";
            static constexpr char ALT[] = "alt";
            static constexpr char SHAPE[] = "shape";
            static constexpr char MATERIAL[] = "material";
        };



        /* ***
         * ATTRIBUTES
         */
        std::string mPath;
        rapidjson::Document mDocument;
        rapidjson::Value mPois;
        rapidjson::Value mCurrentPoi;
        I32 mPoiIndex;
        U32 mPoiCount;

    public:
        /**
         * Creates a new SceneReader ready to parse the file
         * supplied as parameter.
         */
        GeoSceneReader(const std::string& path);
        GeoSceneReader(const GeoSceneReader &) = delete;
        void operator=(const GeoSceneReader &) = delete;
        virtual ~GeoSceneReader();

        /**
         * 1. Open the file
         * 2. Stringify it
         * 3. Create the DOM ready to be queried
         * Must be called before any other method
         */
        Status parse();


        bool hasOrigin() const;
        double getOriginLat() const;
        double getOriginLon() const;

        bool hasCamera() const;
        std::string getCameraType() const;
        glm::dvec3 getCameraCoords() const;


        /**
         * Setup the context to parse the next POI.
         */
        bool nextPoi();
        const std::string getPoiSID() const;
        double getPoiLat() const;
        double getPoiLon() const;
        double getPoiAlt() const;
        std::string getPoiShape() const;
        std::string getPoiMaterial() const;
    };
}

#endif //_DMA_SCENEREADER_HPP
