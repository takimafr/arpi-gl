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


#ifndef _DMA_CUBEMAP_HPP_
#define _DMA_CUBEMAP_HPP_

#include "resource/Texture.hpp"

namespace dma {

    class CubeMap : public Texture {

    public:

        enum Face {
            RIGHT  = 0,
            LEFT   = 1,
            TOP    = 2,
            BOTTOM = 3,
            BACK   = 4,
            FRONT  = 5
        };


        CubeMap();
        virtual ~CubeMap();

        /**
         * From disk
         */
        Status load(const std::string& dirName);

        /**
         * From cache if any
         */
        Status refresh(const std::string& dirName);

    private:

        Status mLoadFromImages();
        void mDeleteImages();

        Image* mImages[6];
    };
}

#endif //_DMA_CUBEMAP_HPP_
