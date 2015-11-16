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


#ifndef _DMA_MATERIAL_HPP_
#define _DMA_MATERIAL_HPP_

#include <cassert>
#include "resource/Texture.hpp"
#include "resource/ShaderProgram.hpp"
#include "resource/Pass.hpp"


namespace dma {
    class Material {
    	static constexpr int DMA_MAX_PASS_COUNT = 4;
        friend class MaterialManager;

    public:

        Material();
        Material(const Material&);
        virtual ~Material();

        inline bool isBackToFront() const { return mBackToFront; }

        inline Pass& getPass(U8 i) {
            assert(i < mPassCount);
            return mPasses[i];
        }

        inline U8 getPassCount() const { return mPassCount; }

        inline const std::string& getSID() { return mSID; }


        inline void setDiffuseMap(std::shared_ptr<Map> diffuseMap, U8 passNum) {
            assert(passNum < mPassCount);
            assert(diffuseMap != 0);
            mPasses[passNum].mDiffuseMap = diffuseMap;
        }

        void addPass(const Pass& pass);

        void reset();

    private:
        //FIELDS
        std::string mSID;
        Pass mPasses[DMA_MAX_PASS_COUNT];
        U8 mPassCount;
        bool mBackToFront;
    };
}

#endif //_DMA_MATERIAL_HPP_
