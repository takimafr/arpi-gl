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



#include "utils/MaterialReader.hpp"
#include "utils/Log.hpp"
#include "utils/ExceptionHandler.hpp"
#include "utils/Utils.hpp"

constexpr auto TAG = "MaterialReader";

constexpr auto B2F_KEY = "backToFront";
constexpr auto PASSES_KEY = "passes";
constexpr auto CULL_MODE_KEY = "cullMode";
constexpr auto DEPTH_KEY = "depthWriting";
constexpr auto SHADER_KEY = "shader";
constexpr auto DIFFUSE_MAP_KEY = "diffuseMap";
constexpr auto LIGHTING_MAP_KEY = "lighting";
constexpr auto SCALING_KEY = "scaling";
constexpr auto DIFFUSE_COLOR_KEY = "diffuseColor";
constexpr auto DM_ACTIVATION_KEY = "diffuseMapActivation";


namespace dma {

    /*======================== PUBLIC ======================*/


    MaterialReader::MaterialReader(const std::string &path) :
            mPath(path),
            mPassCount(0),
            mPassIndex(-1)
    {}



    MaterialReader::~MaterialReader() {

    }



    Status MaterialReader::parse() {
        ////////////////////////////////////////////////////////////////////////////
        // Stringify the file
        std::string json;
        Status status = Utils::bufferize(mPath, json);
        if (status != STATUS_OK) {
            Log::error(TAG, "Unable to bufferize Material %s", mPath.c_str());
            assert(!"Unable to bufferize Material");
            return status;
        }

        ////////////////////////////////////////////////////////////////////////////
        // Create the DOM
        mDocument.Parse(json.c_str());
        if (mDocument.HasParseError()) {
            Log::error(TAG, "Unable to parse Material %s", mPath.c_str());
            assert(!"Unable to parse Material");
            return throwException(TAG, ExceptionType::PARSE_ERROR, "Unable to parse Material " + mPath);
        }

        ////////////////////////////////////////////////////////////////////////////
        // Check if root is an object (vs an array)
        if (!mDocument.IsObject()) {
            Log::error(TAG, "Material file %s has incorrect synthax: root is not an object"
                    , mPath.c_str());
            assert(!"root is not an object");
            return throwException(TAG, ExceptionType::PARSE_ERROR, "Material file " + mPath + " has incorrect synthax : root is not an object");
        }

        ////////////////////////////////////////////////////////////////////////////
        // Check if root has a passes array
        if (!mDocument.HasMember(PASSES_KEY) || !mDocument[PASSES_KEY].IsArray()) {
            Log::error(TAG, "Material file %s has incorrect synthax:"
                    " root does not have any \"passes\" key", mPath.c_str());
            assert(!"root does not have any \"passes\" key");
            return throwException(TAG, ExceptionType::INVALID_VALUE, "Material file " + mPath + " has incorrect synthax : root does not have any \"passes\" key");
        }

        mPasses = mDocument[PASSES_KEY];
        mPassCount = mPasses.Size();

        return STATUS_OK;
    }



    bool MaterialReader::isBackToFront() const {
        if (mDocument.HasMember(B2F_KEY)) {
            const rapidjson::Value& v = mDocument[B2F_KEY];
            if (v.IsNull()) return false;
            if (v.IsBool()) return v.GetBool();
        }
        return false;
    }



    bool MaterialReader::nextPass() {
        mPassIndex += 1;
        return mPassIndex < (I32)mPassCount;
    }



    bool MaterialReader::hasCullMode() const {
        if (mPasses[mPassIndex].HasMember(CULL_MODE_KEY)) {
            const rapidjson::Value& v = mPasses[mPassIndex][CULL_MODE_KEY];
            if (v.IsNull()) return false;
            if (v.IsString()) return true;
        }
        return false;
    }



    std::string MaterialReader::getCullMode() const {
        if (!hasCullMode()) {
            Log::error(TAG, "Material file %s malformed:"
                    " no %s key or the value is not a string nor null", mPath.c_str(), CULL_MODE_KEY);
            assert(!"error while parsing cull mode");
            return "";
        }
        return mPasses[mPassIndex][CULL_MODE_KEY].GetString();
    }



    bool MaterialReader::hasDepthWriting() const {
        if (mPasses[mPassIndex].HasMember(DEPTH_KEY)) {
            const rapidjson::Value& v = mPasses[mPassIndex][DEPTH_KEY];
            if (v.IsNull()) return false;
            if (v.IsBool()) return v.GetBool();
        }
        return true; //default enable depth writing
    }



    std::string MaterialReader::getShader() const {
        if (mPasses[mPassIndex].HasMember(SHADER_KEY) && mPasses[mPassIndex][SHADER_KEY].IsString()) {
            return mPasses[mPassIndex][SHADER_KEY].GetString();
        } else {
            Log::error(TAG, "Material file %s malformed:"
                    " no %s key or the value is not a string", mPath.c_str(), SHADER_KEY);
            assert(!"no key or invalid value");
            return "";
        }
    }



    bool MaterialReader::hasDiffuseMap() const {
        if (mPasses[mPassIndex].HasMember(DIFFUSE_MAP_KEY)) {
            const rapidjson::Value& v = mPasses[mPassIndex][DIFFUSE_MAP_KEY];
            if (v.IsNull()) return false;
            if (v.IsString()) return true;
        }
        return false;
    }



    std::string MaterialReader::getDiffuseMap() const {
        if (!hasDiffuseMap()) {
            Log::error(TAG, "Material file %s malformed:"
                    " no %s key or the value is not a string nor null", mPath.c_str(), DIFFUSE_MAP_KEY);
            assert(!"error while parsing diffuse map");
            return "";
        }
        return mPasses[mPassIndex][DIFFUSE_MAP_KEY].GetString();
    }



    bool MaterialReader::hasLighting() const {
        if (mPasses[mPassIndex].HasMember(LIGHTING_MAP_KEY)) {
            const rapidjson::Value& v = mPasses[mPassIndex][LIGHTING_MAP_KEY];
            if (v.IsNull()) return false;
            if (v.IsString()) return true;
        }
        return false;
    }



    std::string MaterialReader::getLighting() const {
        if (!hasLighting()) {
            Log::error(TAG, "Material file %s malformed:"
                    " no %s key or the value is not a string nor null", mPath.c_str(), LIGHTING_MAP_KEY);
            assert(!"error while parsing lighting");
            return "";
        }
        return mPasses[mPassIndex][LIGHTING_MAP_KEY].GetString();
    }



    bool MaterialReader::hasScaling() const {
        if (mPasses[mPassIndex].HasMember(SCALING_KEY)) {
            const rapidjson::Value& v = mPasses[mPassIndex][SCALING_KEY];
            if (v.IsNull()) return false;
            if (v.IsBool()) return v.GetBool();
        }
        return false;
    }



    bool MaterialReader::hasDiffuseColor() const {
        if (mPasses[mPassIndex].HasMember(DIFFUSE_COLOR_KEY)) {
            const rapidjson::Value& v = mPasses[mPassIndex][DIFFUSE_COLOR_KEY];
            if (v.IsNull()) return false;
            if (v.IsArray()) return true;
        }
        return false;
    }



    const glm::vec3 MaterialReader::getDiffuseColor() const {
        if (!hasDiffuseColor()) {
            Log::error(TAG, "Material file %s malformed:"
                    " no %s key or the value is not a string nor null", mPath.c_str(), DIFFUSE_COLOR_KEY);
            assert(!"error while parsing diffuse color");
            //return NULL;
        }

        const rapidjson::Value& colors = mPasses[mPassIndex][DIFFUSE_COLOR_KEY];
        glm::vec3 res(colors[0].GetDouble(), colors[1].GetDouble(), colors[2].GetDouble());
        return res;
    }



    bool MaterialReader::hasDiffuseMapActivation() {
        if (mPasses[mPassIndex].HasMember(DM_ACTIVATION_KEY)) {
            const rapidjson::Value& v = mPasses[mPassIndex][DM_ACTIVATION_KEY];
            if (v.IsNull()) return false;
            if (v.IsBool()) return v.GetBool();
        }
        return false;
    }
}
