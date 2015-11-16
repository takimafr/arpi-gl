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



#include "utils/GeoSceneReader.hpp"
#include "utils/Log.hpp"
#include "utils/ExceptionHandler.hpp"
#include "utils/Utils.hpp"



namespace dma {

    constexpr char GeoSceneReader::TAG[];


    constexpr char GeoSceneReader::Keys::ORIGIN[];
    constexpr char GeoSceneReader::Keys::CAMERA[];
    constexpr char GeoSceneReader::Keys::CAMERA_TYPE[];
    constexpr char GeoSceneReader::Keys::POIS[];
    constexpr char GeoSceneReader::Keys::SID[];
    constexpr char GeoSceneReader::Keys::LAT[];
    constexpr char GeoSceneReader::Keys::LON[];
    constexpr char GeoSceneReader::Keys::ALT[];
    constexpr char GeoSceneReader::Keys::SHAPE[];
    constexpr char GeoSceneReader::Keys::MATERIAL[];

    //--------------------------------------------------------------------------------
    GeoSceneReader::GeoSceneReader(const std::string &path) :
                                mPath(path),
                                mPoiIndex(-1),
                                mPoiCount(0)
    {}


    //--------------------------------------------------------------------------------
    GeoSceneReader::~GeoSceneReader() {

    }

    //--------------------------------------------------------------------------------
    Status GeoSceneReader::parse() {
        ////////////////////////////////////////////////////////////////////////////
        // Stringify the file
        std::string json;
        Status status = Utils::bufferize(mPath, json);
        if (status != STATUS_OK) {
            Log::error(TAG, "Unable to bufferize Scene %s", mPath.c_str());
            assert(!"Unable to bufferize Scene");
            return status;
        }

        ////////////////////////////////////////////////////////////////////////////
        // Create the DOM
        mDocument.Parse(json.c_str());
        if (mDocument.HasParseError()) {
            Log::error(TAG, "Unable to parse Scene %s", mPath.c_str());
            assert(!"Unable to parse Scene");
            return throwException(TAG, ExceptionType::PARSE_ERROR, "Unable to parse Scene " + mPath);
        }

        ////////////////////////////////////////////////////////////////////////////
        // Check if root is an object (vs an array)
        if (!mDocument.IsObject()) {
            Log::error(TAG, "Scene file %s has incorrect syntax:"
                    " root is not an object", mPath.c_str());
            assert(!"root is not an object");
            return throwException(TAG, ExceptionType::PARSE_ERROR, "Scene file " + mPath + " has incorrect syntax : root is not an object");
        }

        ////////////////////////////////////////////////////////////////////////////
        // Check if root has a entity array
        if (mDocument.HasMember(Keys::POIS) && mDocument[Keys::POIS].IsArray()) {
            mPois = mDocument[Keys::POIS];
            mPoiCount = mPois.Size();
        }
        return STATUS_OK;
    }


    //--------------------------------------------------------------------------------
    bool GeoSceneReader::hasOrigin() const {
        if (mDocument.HasMember(Keys::ORIGIN)) {
            const rapidjson::Value& v = mDocument[Keys::ORIGIN];
            if(v.IsNull()) return false;
            if(v.IsObject()) return true;
        }
        return false;
    }


    //--------------------------------------------------------------------------------
    double GeoSceneReader::getOriginLat() const {
        if (!hasOrigin()) {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string nor null", mPath.c_str(), Keys::ORIGIN);
            assert(!"error while parsing scene origin");
            return 0.0;
        }
        const rapidjson::Value& origin = mDocument[Keys::ORIGIN];
        return origin[Keys::LAT].GetDouble();
    }


    //--------------------------------------------------------------------------------
    double GeoSceneReader::getOriginLon() const {
        if (!hasOrigin()) {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string nor null", mPath.c_str(), Keys::ORIGIN);
            assert(!"error while parsing scene origin");
            return 0.0;
        }
        const rapidjson::Value& origin = mDocument[Keys::ORIGIN];
        return origin[Keys::LON].GetDouble();
    }


    //--------------------------------------------------------------------------------
    bool GeoSceneReader::hasCamera() const {
        if (mDocument.HasMember(Keys::CAMERA)) {
            const rapidjson::Value& v = mDocument[Keys::CAMERA];
            if(v.IsNull()) return false;
            if(v.IsObject()) return true;
        }
        return false;
    }


    //--------------------------------------------------------------------------------
    std::string GeoSceneReader::getCameraType() const {
        if (!hasCamera()) {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string nor null", mPath.c_str(), Keys::CAMERA);
            assert(!"error while parsing scene camera");
            return "";
        }
        const rapidjson::Value& camera = mDocument[Keys::CAMERA];
        return camera[Keys::CAMERA_TYPE].GetString();
    }


    //--------------------------------------------------------------------------------
    glm::dvec3 GeoSceneReader::getCameraCoords() const {
        glm::dvec3 coords(0.0);
        if (!hasCamera()) {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string nor null", mPath.c_str(), Keys::CAMERA);
            assert(!"error while parsing scene camera");
            return coords;
        }
        const rapidjson::Value& camera = mDocument[Keys::CAMERA];
        coords.x = camera[Keys::LAT].GetDouble();
        coords.y = camera[Keys::LON].GetDouble();
        coords.z = camera[Keys::ALT].GetDouble();
        return coords;
    }


    //--------------------------------------------------------------------------------
    bool GeoSceneReader::nextPoi() {
        mPoiIndex += 1;
        if(mPoiIndex >= (I32)mPoiCount) return false;
        mCurrentPoi = mPois[mPoiIndex];
        return true;
    }


    //--------------------------------------------------------------------------------
    double GeoSceneReader::getPoiLat() const {
        if (mCurrentPoi.HasMember(Keys::LAT) && mCurrentPoi[Keys::LAT].IsDouble()) {
            return mCurrentPoi[Keys::LAT].GetDouble();
        } else {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string", mPath.c_str(), Keys::LAT);
            assert(!"no key or invalid value");
            return 0.0;
        }
    }


    //--------------------------------------------------------------------------------
    double GeoSceneReader::getPoiLon() const {
        if (mCurrentPoi.HasMember(Keys::LON) && mCurrentPoi[Keys::LON].IsDouble()) {
            return mCurrentPoi[Keys::LON].GetDouble();
        } else {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string", mPath.c_str(), Keys::LON);
            assert(!"no key or invalid value");
            return 0.0;
        }
    }


    //--------------------------------------------------------------------------------
    double GeoSceneReader::getPoiAlt() const {
        if (mCurrentPoi.HasMember(Keys::ALT) && mCurrentPoi[Keys::ALT].IsDouble()) {
            return mCurrentPoi[Keys::ALT].GetDouble();
        } else {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string", mPath.c_str(), Keys::ALT);
            assert(!"no key or invalid value");
            return 0.0;
        }
    }


    //--------------------------------------------------------------------------------
    std::string GeoSceneReader::getPoiShape() const {
        if (mCurrentPoi.HasMember(Keys::SHAPE) && mCurrentPoi[Keys::SHAPE].IsString()) {
            return mCurrentPoi[Keys::SHAPE].GetString();
        } else {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string", mPath.c_str(), Keys::SHAPE);
            assert(!"no key or invalid value");
            return "";
        }
    }


    //--------------------------------------------------------------------------------
    std::string GeoSceneReader::getPoiMaterial() const {
        if (mCurrentPoi.HasMember(Keys::MATERIAL) && mCurrentPoi[Keys::MATERIAL].IsString()) {
            return mCurrentPoi[Keys::MATERIAL].GetString();
        } else {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string", mPath.c_str(), Keys::MATERIAL);
            assert(!"no key or invalid value");
            return "";
        }
    }

    const std::string GeoSceneReader::getPoiSID() const {
        if (mCurrentPoi.HasMember(Keys::SID) && mCurrentPoi[Keys::SID].IsString()) {
            return mCurrentPoi[Keys::SID].GetString();
        } else {
            Log::error(TAG, "Scene file %s malformed:"
                    " no %s key or the value is not a string", mPath.c_str(), Keys::SID);
            assert(!"no key or invalid value");
            return "";
        }
    }
}
