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


#ifndef _DMA_MAP_HPP_
#define _DMA_MAP_HPP_

#include "resource/Texture.hpp"
#include "utils/GLES2Logger.hpp"

namespace dma {

    class Map : public Texture {

    public:
        Map();
        virtual ~Map();

        /**
         * Set the Image cache.
         * /!\ The Map is now responsible for the image life span. (ie: do not delete the pointer yourself)
         */
        inline void setImage(Image* image) {
            mImage = image;
        }

        Status load(const std::string& filename);
        /**
         * Loads the map from the provided Image.
         * A copy will be kept in cache.
         */
        Status load(const Image& image);
        Status refresh(const std::string &filename);
        Status refresh();

    private:

        Status mLoadFromImage();

        Image* mImage;

    };
}

#endif //_DMA_MAP_HPP_
