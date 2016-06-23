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



#include "resource/CubeMap.hpp"
#include "utils/Log.hpp"
#include <vector>
#include <cassert>

constexpr auto TAG = "CubeMap";

namespace dma {


    CubeMap::CubeMap() :
            Texture()
    {
        for (U32 i = 0; i < 6; ++i) {
            mImages[i] = nullptr;
        }
    }


    CubeMap::~CubeMap() {
        clearCache();
    }

    void CubeMap::clearCache() {
        for (U32 i = 0; i < 6; ++i) {
            delete mImages[i];
            mImages[i] = nullptr;
        }
    }



    void CubeMap::load(const std::string& dirName) {
        std::vector<std::string> faces;
        faces.push_back(dirName + "/right.png");
        faces.push_back(dirName + "/left.png");
        faces.push_back(dirName + "/top.png");
        faces.push_back(dirName + "/bottom.png");
        faces.push_back(dirName + "/back.png");
        faces.push_back(dirName + "/front.png");

        Log::trace(TAG, "Loading Cube Map %s ...", dirName.c_str());

        if (mImages[0] != nullptr) {
            Log::trace(TAG, "Refreshing CubeMap %s using cache", dirName.c_str());
            mLoadFromImages();
        } else {
            for (GLuint i = 0; i < faces.size(); i++) {
                Image *img = new Image();
                mImages[i] = img;
                img->loadAsPNG(faces[i], false);
            }
           mLoadFromImages();
        }
        Log::trace(TAG, "Cube Map %s loaded", dirName.c_str());
        //TODO clear images if cache is off
    }



    void CubeMap::refresh(const std::string &dirName) {
        if (mImages[0] == nullptr) {
            Log::trace(TAG, "Refreshing CubeMap %s from disk", dirName.c_str());
            return load(dirName);
        } else {
            Log::trace(TAG, "Refreshing CubeMap %s using cache", dirName.c_str());
            return mLoadFromImages();
        }
    }


    void CubeMap::mLoadFromImages() {
        glGenTextures(1, &mHandle);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
        for (GLuint i = 0; i < 6; i++) {
            Image* img =  mImages[i];

            /* upload texture data */
            GLint maxTextureSize;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
            assert((U32)maxTextureSize > img->getWidth());
            assert((U32)maxTextureSize > img->getHeight());

            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, //mipmap level
                    img->getFormat(),
                    img->getWidth(),
                    img->getHeight(),
                    0, //ES border must be 0
                    (GLenum)img->getFormat(),
                    GL_UNSIGNED_BYTE,
                    img->getPixels());
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R_OES, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0); //unbind texture

        //TODO clear images if cache is off
    }
}
