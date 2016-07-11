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


#ifndef _DMA_IMAGE_HPP
#define _DMA_IMAGE_HPP

#include "common/Types.hpp"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <libpng16/png.h>
#include <string>

namespace dma {
    class Image {
    public:
        Image();
        Image(const Image& other);
        /**
         * Creates a new Image width * height from the provided pixels.
         * A copy of pixels is made.
         */
        Image(U32 width, U32 height, GLint format, BYTE* pixels);
        ~Image();

    public:
        void loadAsPNG(const std::string& filename);
        void loadAsPNG(const std::string&filename, bool reverse);
        Status loadAsPNG(BYTE* data);

        U32 getWidth();
        U32 getHeight();
        GLint getFormat();
        BYTE* getPixels();

    private:
        void mReadPngData(png_struct* png_ptr, GLubyte* data, bool reverse);

    private:
        U32 mWidth;
        U32 mHeight;
        GLint mFormat;
        U32 mBytesPerPixel;
        BYTE* mPixels;
    };
}

#endif /* _DMA_IMAGE_HPP */
