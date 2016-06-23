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



#include "resource/Texture.hpp"


constexpr auto TAG = "Texture";


namespace dma {


    Texture::Texture() :
            mHandle(0)
    {}


    void Texture::wipe() {
        Log::trace(TAG, "Trying to delete texture handle %d", mHandle);
        if (mHandle != 0 && glIsTexture(mHandle) == GL_TRUE) {
            Log::trace(TAG, "Deleting texture handle %d", mHandle);
            glDeleteTextures(1, &mHandle);
            mHandle = 0;
        }
    }

} /* namespace dma */
