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



#include "resource/Material.hpp"

namespace dma {


    Material::Material() :
            mPassCount(0), mBackToFront(false)
    {}



    Material::Material(const Material &material) {
        this->mSID = material.mSID;
        this->mPassCount = material.mPassCount;
        for (int i = 0; i < mPassCount; ++i) {
            mPasses[i] = material.mPasses[i];
        }
        this->mBackToFront = material.mBackToFront;
    }

    void Material::addPass(const Pass& pass) {
        mPasses[mPassCount++] = pass;
    }



}
