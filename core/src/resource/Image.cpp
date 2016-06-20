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



#include "resource/Image.hpp"
#include "utils/Log.hpp"
#include "utils/ExceptionHandler.hpp"
#include "utils/Utils.hpp"
#include <fstream>

#include <string.h>
#include <pngconf.h>


constexpr auto TAG = "Image";

namespace dma {

    struct ByteBuffer {
        U32 offset;
        BYTE* data;
    };

    //============================ ROUTINES ================================//

    void memoryReadCallback(png_structp png, png_bytep data, png_size_t size) {
        ByteBuffer* userData = ((ByteBuffer*)png_get_io_ptr(png));
        memcpy(data, userData->data + userData->offset, size);
        userData->offset += size;
    }



    inline void onPngError(FILE* file, const std::string& filename, const std::string& error) {
        fclose (file);
        Log::error(TAG, "error processing file \"%s\" : %s", filename.c_str(), error.c_str());
        throwException(TAG, ExceptionType::IO, "error processing file texture file");
    }



    inline void  normalizePngInfo(png_struct* png_ptr, png_info* info_ptr) {
        int bit_depth, color_type;

        /* get some usefull information from header */
        bit_depth = png_get_bit_depth (png_ptr, info_ptr);
        color_type = png_get_color_type (png_ptr, info_ptr);

        /* convert index color images to RGB images */
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png_ptr);
        }

        /* convert 1-2-4 bits grayscale images to 8 bits
                                   grayscale. */
        if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }

        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha (png_ptr);
        }

        /* make each canal to use exactly 8 bits */
        if (bit_depth == 16) {
            png_set_strip_16 (png_ptr);
        } else if (bit_depth < 8) {
            png_set_packing (png_ptr);
        }

        /* update info structure to apply transformations */
        png_read_update_info(png_ptr, info_ptr);
    }



    // TODO : scale down if > GL_MAX_TEXTURE_SIZE
    bool checkSizePowOf2(U32 width, U32 height) {

        bool res = true;
        for(U32 x : {width, height}) {
            /* While x is even and > 1 */
            while (((x % 2) == 0) && x > 1) {
                x /= 2;
            }
            res &= (x == 1);
        }
        return res;
    }



    void png_error_fn (png_structp png_ptr, png_const_charp error_msg) {
        Log::error(TAG, "png_error: %s (%s)", error_msg, (char *)png_get_error_ptr (png_ptr));
        longjmp (png_jmpbuf (png_ptr), 1);
    }



    void png_warning_fn (png_structp png_ptr, png_const_charp warning_msg) {
        Log::warn(TAG, "png_error: %s (%s)", warning_msg, (char *)png_get_error_ptr (png_ptr));
    }


    //===========================================================================//


    Image::Image() :
            mWidth(0), mHeight(0),
            mFormat(0), mBytesPerPixel(0), mPixels(NULL)
    {}



    Image::Image(const Image &other) :
        mWidth(other.mWidth),
        mHeight(other.mHeight),
        mFormat(other.mFormat),
        mBytesPerPixel(other.mBytesPerPixel),
        mPixels(nullptr)
    {
        if (other.mPixels != nullptr) {
            U32 size = mWidth * mHeight * mBytesPerPixel;
            mPixels = new GLubyte[size];
            memcpy(mPixels, other.mPixels, size);
        }
    }



    Image::Image(U32 width, U32 height, GLint format, BYTE* pixels) {
        mWidth = width;
        mHeight = height;
        mFormat = format;
        switch (format) {
            case GL_LUMINANCE:
                mBytesPerPixel = 1;
                break;

            case GL_LUMINANCE_ALPHA:
                mBytesPerPixel = 2;
                break;

            case GL_RGB:
                mBytesPerPixel = 3;
                break;

            case GL_RGBA:
                mBytesPerPixel = 4;
                break;

            default:
                Log::error(TAG, "unknown PNG color format : %d ", format);
                assert(!"unknown PNG color format");
                break;
        }
        if (pixels != nullptr) {
            U32 size = mWidth * mHeight * mBytesPerPixel;
            mPixels = new GLubyte[size];
            memcpy(mPixels, pixels, size);
        }
    }



    Image::~Image(){
        delete[] mPixels;
    }


    U32 Image::getWidth(){ return mWidth;}
    U32 Image::getHeight(){return mHeight;}
    GLint Image::getFormat(){return mFormat;}
    BYTE* Image::getPixels(){return mPixels;}



    void Image::mReadPngData(png_struct* png_ptr, GLubyte* data, bool reverse) {
        png_bytep *row_pointers;

        /* setup a pointer array.  Each one points at the beginning of a row. */
        row_pointers = new png_bytep[mHeight];

        if (reverse) {
            for (unsigned int i = 0; i < mHeight; ++i) {
                row_pointers[i] = (png_bytep) (data +
                                               ((mHeight - (i + 1)) * mWidth * mBytesPerPixel));
            }
        } else {
            for (unsigned int i = 0; i < mHeight; ++i) {
                row_pointers[i] = (png_bytep) (data + i * mWidth * mBytesPerPixel);
            }
        }

        /* read pixel data using row pointers, to start reading from the bottom of the image. */
        png_read_image(png_ptr, row_pointers);

        /* we don't need row pointers anymore */
        delete[] row_pointers;
    }



    Status Image::loadAsPNG(const std::string& filename) {
        std::string fname = filename;
        Utils::addFileExt(fname, "png");
        return loadAsPNG(filename, true);
    }



    Status Image::loadAsPNG(const std::string &filename, bool reverse) {

        std::string fname = filename;
        Utils::addFileExt(fname, "png");

        FILE *file;

        // png stuff
        png_structp png_ptr;
        png_infop info_ptr;
        png_byte magic[8];

        // image parameters
        int bit_depth, color_type;

        /* open texture data read / binary */
        file = fopen(fname.c_str(), "rb");

        if (!file) {
            Log::error(TAG, "file %s doesn't exist", fname.c_str());
            return throwException(TAG, ExceptionType::IO, "cannot open file " + fname);
        }

        /* read magic number to ensure this file is a png */
        if (fread (magic, sizeof (magic), 1, file) <= 0) {
            fclose(file);
            Log::error(TAG, "cannot read \"%s\" magic number", fname.c_str());
            return throwException(TAG, ExceptionType::INVALID_FILE, "cannot read texture file");
        }

        /* check for valid magic number */
        if (!png_check_sig (magic, sizeof (magic))) {
            onPngError(file, fname, "is not a valid PNG file");
            return throwException(TAG, ExceptionType::INVALID_FILE, (fname + " is not a valid PNG file").c_str());
        }

        /* create a png read struct */
        png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING,
                                          (png_voidp *) fname.c_str(),
                                          png_error_fn,               // error callback
                                          png_warning_fn);            // warning callback

        if (!png_ptr) {
            onPngError(file, fname, "cannot create data structure");
            return throwException(TAG, ExceptionType::MEMORY, "cannot create data structure");
        }

        /* create a png info struct */
        info_ptr = png_create_info_struct (png_ptr);
        if (!info_ptr) {
            onPngError(file, fname, "cannot read info");
            return throwException(TAG, ExceptionType::INVALID_FILE, "cannot read info");
        }

        // initialize the setjmp for returning properly after a libpng error occurred
        if (setjmp (png_jmpbuf (png_ptr))) {
            png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
            onPngError(file, fname, "unknown error");
            return throwException(TAG, ExceptionType::UNKNOWN, "unknown error");
        }

        // setup libpng for using standard C fread() function
        // with our FILE pointer
        png_init_io(png_ptr, file);

        /* tell libpng that we have already read the magic number */
        png_set_sig_bytes(png_ptr, sizeof (magic));

        /* read png info */
        png_read_info(png_ptr, info_ptr);

        /* normalize & update png info,
         * in order that every PNG image read use the same parameters. */
        normalizePngInfo(png_ptr, info_ptr);

        /* create our texture object. */
        //texture = new Texture();

        /* retrieve updated information in IHDR (png header) */
        png_get_IHDR (png_ptr, info_ptr,
                      (png_uint_32*)(&mWidth),
                      (png_uint_32*)(&mHeight),
                      &bit_depth, &color_type,
                      NULL, NULL, NULL);

        Log::trace(TAG, "loading texture of size (%d, %d)", mWidth, mHeight);

        if(!checkSizePowOf2(mWidth, mHeight)) {

            std::stringstream ss;

            ss << "texture size (" << mWidth << ", " << mHeight << ")" << " must be power of 2";
            Log::error(TAG, "%s", ss.str().c_str());
            assert(!"size must be a power of 2");
            return throwException(TAG, ExceptionType::INVALID_FILE, ss.str());
        }

        /* convert PNG color-type to openGL texture format. */
        /* deduce GL Internal format from PNG format. */
        switch (color_type) {
            case PNG_COLOR_TYPE_GRAY:
                mFormat = GL_LUMINANCE;
                mBytesPerPixel = 1;
                break;

            case PNG_COLOR_TYPE_GRAY_ALPHA:
                mFormat = GL_LUMINANCE_ALPHA;
                mBytesPerPixel = 2;
                break;

            case PNG_COLOR_TYPE_RGB:
                mFormat = GL_RGB;
                mBytesPerPixel = 3;
                break;

            case PNG_COLOR_TYPE_RGB_ALPHA:
                mFormat = GL_RGBA;
                mBytesPerPixel = 4;
                break;

            default:
                Log::error(TAG, "unknown PNG color format : %d ", color_type);
                assert(!"unknown PNG color format");
                break;
        }

        /* we can now allocate memory for storing pixel data */
        mPixels = new GLubyte[mWidth *
                              mHeight *
                              mBytesPerPixel];
        assert(mPixels && "cannot alloc Gl texture");

        /* read png data & fill data array */
        mReadPngData(png_ptr, mPixels, reverse);

        /* finish decompression and release memory */
        png_read_end(png_ptr, NULL);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(file);

        return STATUS_OK;
    }



    Status Image::loadAsPNG(BYTE* data) {
        png_byte header[8];
        png_structp pngPtr = NULL;
        png_infop infoPtr = NULL;
        png_bytep* rowPtrs = NULL;
        png_int_32 rowSize;
        bool transparency;

        ///////////////////////////////////////////////:
        // Check the header signature
        memcpy(header, data, sizeof(header));
        if (png_sig_cmp(header, 0, 8) != 0) goto ERROR;

        pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                        NULL,
                                        png_error_fn,           // error callback
                                        png_warning_fn);        // warning callback
        if (!pngPtr) goto ERROR;

        infoPtr = png_create_info_struct(pngPtr);
        if (!infoPtr) goto ERROR;

        if (setjmp(png_jmpbuf(pngPtr))) goto ERROR;

        ////////////////////////////////////////////////////////////////////////
        // Create the read structure and set the read function from a memory pointer
        ByteBuffer bb;
        bb.offset = 8; //sig
        bb.data = data;
        png_set_read_fn(pngPtr, &bb, memoryReadCallback);

        //tell libpng we already read the signature
        png_set_sig_bytes(pngPtr, 8);

        png_read_info(pngPtr, infoPtr);

        png_int_32 depth, colorType;
        png_uint_32 width, height;
        png_get_IHDR(pngPtr, infoPtr, &width, &height,
                     &depth, &colorType, NULL, NULL, NULL);
        mWidth = width;
        mHeight = height;

        // Creates a full alpha channel if transparency is encoded as
        // an array of palette entries or a single transparent color.
        transparency = false;
        if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(pngPtr);
            transparency = true;
        }
        // Expands PNG with less than 8bits per channel to 8bits.
        if (depth < 8) {
            png_set_packing(pngPtr);
        }
            // Shrinks PNG with 16bits per color channel down to 8bits.
        else if (depth == 16){
            png_set_strip_16(pngPtr);
        }
        // Indicates that image needs conversion to RGBA if needed.
        switch (colorType){
            case PNG_COLOR_TYPE_PALETTE:
                png_set_palette_to_rgb(pngPtr);
                if (transparency) {
                    mFormat = GL_RGBA;
                    mBytesPerPixel = 4;
                } else {
                    mFormat = GL_RGB;
                    mBytesPerPixel = 3;
                }
                break;
            case PNG_COLOR_TYPE_RGB:
                if (transparency) {
                    mFormat = GL_RGBA;
                    mBytesPerPixel = 4;
                } else {
                    mFormat = GL_RGB;
                    mBytesPerPixel = 3;
                }
                break;
            case PNG_COLOR_TYPE_RGBA:
                if (transparency) {
                    mFormat = GL_RGBA;
                    mBytesPerPixel = 4;
                } else {
                    mFormat = GL_RGB;
                    mBytesPerPixel = 3;
                }
                break;
            case PNG_COLOR_TYPE_GRAY:
                png_set_expand_gray_1_2_4_to_8(pngPtr);
                mFormat = transparency  ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
                if (transparency) {
                    mFormat = GL_LUMINANCE_ALPHA;
                    mBytesPerPixel = 2;
                } else {
                    mFormat = GL_LUMINANCE;
                    mBytesPerPixel = 1;
                }
                break;
            case PNG_COLOR_TYPE_GA:
                png_set_expand_gray_1_2_4_to_8(pngPtr);
                if (transparency) {
                    mFormat = GL_LUMINANCE_ALPHA;
                    mBytesPerPixel = 2;
                } else {
                    mFormat = GL_LUMINANCE;
                    mBytesPerPixel = 1;
                }
                break;
            default:
                assert(false);
                break;
        }
        png_read_update_info(pngPtr, infoPtr);

        rowSize = png_get_rowbytes(pngPtr, infoPtr);
        if(rowSize <= 0) goto ERROR;
        mPixels = new BYTE[rowSize * height];
        if(!mPixels) goto ERROR;
        rowPtrs = new png_bytep[height];
        if(!rowPtrs) goto ERROR;

        for(U32 i = 0; i < height; ++i){
            rowPtrs[height - (i + 1)] = mPixels + i * rowSize;
        }
        png_read_image(pngPtr, rowPtrs);

        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
        delete[] rowPtrs;
        return STATUS_OK;

        ERROR:
        Log::error(TAG, "Error while reading PNG data");
        delete[] rowPtrs; delete[] mPixels;
        if(pngPtr != NULL){
            png_infop* infoPtrP = infoPtr != NULL ? &infoPtr : NULL;
            png_destroy_read_struct(&pngPtr, infoPtrP, NULL);
        }
        return throwException(TAG, ExceptionType::INVALID_FILE, "unknown error while reading PNG data");
    }

}
