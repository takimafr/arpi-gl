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



#include "resource/Pass.hpp"

namespace dma {


    Pass::Pass() :
            mFuncFlags(0),
            mCullMode(NONE),
            mDepthWriting(true),
            mShaderProgram(nullptr),
            mDiffuseMap(nullptr),
            mDiffuseMapEnabled(false)
    {}



    Pass::~Pass() {

    }



    void Pass::addFunc(Pass::Func func) {
        mFuncFlags |= (1L << func);
    }



    void Pass::removeFunc(Pass::Func func) {
        mFuncFlags &= ~(1L << func);
    }



    void Pass::setShaderProgram(std::shared_ptr<ShaderProgram> shaderProgram) {
        mShaderProgram = shaderProgram;
    }



    void Pass::setDiffuseMap(std::shared_ptr<Map> diffuseMap)  {
        mDiffuseMap = diffuseMap;
        addFunc(Func::DIFFUSE_MAP);
        mDiffuseMapEnabled = true;
    }
}
