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


#ifndef _DMA_PASS_HPP_
#define _DMA_PASS_HPP_

#include "common/Types.hpp"
#include "resource/ShaderProgram.hpp"
#include "resource/Texture.hpp"
#include "resource/Map.hpp"

#include <memory>

namespace dma {

    class Pass {

        friend class MaterialManager;
        friend class Material;

    public:
        enum Func {
            TRANSFORM = 0,       //means translation, rotation, scaling
            LIGHTING_FLAT = 1,   //means lighting computation (gouraud, phong or the like)
            LIGHTING_SMOOTH = 2, //means lighting computation (gouraud, phong or the like)
            DIFFUSE_MAP = 3,     //means uses a diffuse map
            DIFFUSE_MAP_ACTIVATION = 4, //means can enable or disable a diffuse map
            SCALING = 5,         //means uses smooth scaling
            DIFFUSE_COLOR = 6,   //means uses a diffuse color
        };

        enum CullMode {
            NONE = 0,
            FRONT = 1,
            BACK = 2
        };

        virtual ~Pass();

        inline std::shared_ptr<ShaderProgram> getShaderProgram() const {
            return mShaderProgram;
        }

        inline CullMode getCullMode() const {
            return mCullMode;
        }

        inline void setCullMode(CullMode cullMode) {
            mCullMode = cullMode;
        }

        inline bool getDepthWriting() const {
            return mDepthWriting;
        }

        inline std::shared_ptr<Map> getDiffuseMap() const {
            return mDiffuseMap;
        }

        void setDiffuseMap(std::shared_ptr<Map> diffuseMap);

        inline void setDiffuseMapEnabled(bool enabled) {
            mDiffuseMapEnabled = enabled;
        }

        inline bool isDiffuseMapEnabled() {
            return mDiffuseMapEnabled;
        }

        inline const glm::vec3& getDiffuseColor() const {
            return mDiffuseColor;
        }

        inline void setDiffuseColor(const glm::vec3& diffuseColor) {
            mDiffuseColor = diffuseColor;
            addFunc(Func::DIFFUSE_COLOR);
        }

        inline void setLightingMode(Pass::Func lightingMode) {
            switch (lightingMode) {
                case LIGHTING_FLAT:
                    removeFunc(Func::LIGHTING_SMOOTH);
                    break;
                case LIGHTING_SMOOTH:
                    removeFunc(Func::LIGHTING_FLAT);
                    break;
                default:
                    Log::warn("Pass", "Wrong lighting mode. Has no effect");
                    assert(false);
                    return;
            }
            addFunc(lightingMode);
        }


        inline bool hasFunc(Func func) const {
            return (mFuncFlags & (1L << func)) != 0;
        }

    private:
        void addFunc(Func func);
        void removeFunc(Func func);

        void setShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram);

        Pass();

        //FIELDS
        U32 mFuncFlags;
        CullMode mCullMode;
        bool mDepthWriting;
        std::shared_ptr<ShaderProgram> mShaderProgram;
        std::shared_ptr<Map> mDiffuseMap;
        bool mDiffuseMapEnabled;
        glm::vec3 mDiffuseColor;
    };
}

#endif //_DMA_PASS_HPP_
