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



#include "resource/Map.hpp"
#include "utils/ExceptionHandler.hpp"

constexpr auto TAG = "Map";

namespace dma {

    static bool enableAnisotropy = false;


    //----------------------------------------------------------------------------------------------
    inline void checkAnisotropyExt() {
        static bool mChecked = false;
        if(mChecked) {
            return;
        }
        mChecked = true;
        if(GLUtils::isExtSupported("GL_EXT_texture_filter_anisotropic")) {
            enableAnisotropy = true;
        } else {
            enableAnisotropy = false;
            Log::warn(TAG, "Anisotropic filering not supported on this platform.");
        }
    }


    //---------------------------------------------------------------------
    Map::Map() :
            Texture(),
            mImage(nullptr)
    {}


    //---------------------------------------------------------------------
    Map::~Map() {
        delete mImage;
    }


    //---------------------------------------------------------------------
    Status Map::load(const std::string& filename) {

        Log::trace(TAG, "Loading 2D texture %s ...", filename.c_str());

        if (mImage != nullptr) delete mImage;
        mImage = new Image();
        Status status = mImage->loadAsPNG(filename) ;
        if (status != STATUS_OK) {
            Log::error(TAG, "Unable to load map %s" , filename.c_str());
            return status;
        }
        status = mLoadFromImage();
        if (status != STATUS_OK) {
            Log::error(TAG, "Unable to load map %s", filename.c_str());
            return status;
        }

        Log::trace(TAG, "2D texture %s loaded", filename.c_str());

        //TODO delete mImage if cache is off
        return STATUS_OK;
    }


    //---------------------------------------------------------------------
    Status Map::load(const Image &image) {
        Log::trace(TAG, "Loading 2D texture from Image");


        if (mImage != nullptr) delete mImage;
        mImage = new Image(image);

        Status status = mLoadFromImage() ;
        if (status != STATUS_OK) {
            Log::error(TAG, "Unable to load map from image");
            assert("Unable to load map from image");
            throwException(TAG, ExceptionType::UNKNOWN, "Unable to load map from image");
            return status;
        }
        Log::trace(TAG, "2D texture loaded");
        return STATUS_OK;
    }


    //---------------------------------------------------------------------
    Status Map::refresh(const std::string &filename) {
        if (mImage == nullptr) {
            Log::trace(TAG, "Refreshing Map %s from disk", filename.c_str());
            return load(filename);
        } else {
            Log::trace(TAG, "Refreshing Map %s from cache", filename.c_str());
            return mLoadFromImage();
        }
    }


    //---------------------------------------------------------------------
    Status Map::refresh() {
        if (mImage == nullptr) {
            Log::error(TAG, "Refreshing Map that doesn't have cache");
            assert(!"Refreshing Map that doesn't have cache");
            return throwException(TAG, ExceptionType::UNKNOWN, "Refreshing Map that doesn't have cache");
        } else {
            Log::trace(TAG, "Refreshing Map %s from cache", getSID().c_str());
            return mLoadFromImage();
        }
    }


    //---------------------------------------------------------------------
    Status Map::mLoadFromImage() {
        /* generate texture */
        glGenTextures (1, &mHandle);
        Log::trace(TAG, "(GL texture handle : %d)", mHandle);
        assert(mHandle);
        glBindTexture(GL_TEXTURE_2D, mHandle);

        /* setup texture filters */
        /* texture should tile */
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

//        Log::debug(TAG, "creating GL texture: ");
//        Log::debug(TAG, "format = %d : ",mImage->getFormat());

        /* upload texture data */
        GLint maxTextureSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        assert((U32)maxTextureSize > mImage->getWidth() && "maxTextureSize");
        assert((U32)maxTextureSize > mImage->getHeight() && "maxTextureSize");

        const int mipmapLevel = 0;
        glTexImage2D (GL_TEXTURE_2D,
                      mipmapLevel,
                      mImage->getFormat(),
                      mImage->getWidth(),
                      mImage->getHeight(),
                      0, //ES border must be 0
                      (GLenum)mImage->getFormat(),
                      GL_UNSIGNED_BYTE,
                      mImage->getPixels());

        // Generate mipmaps, by the way.
        glGenerateMipmap(GL_TEXTURE_2D);

        checkAnisotropyExt();
        if (enableAnisotropy) {
            GLfloat anisotropyMax;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropyMax);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyMax );
        }
        glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

        //TODO delete mImage if cache is off
        return STATUS_OK;
    }
}
