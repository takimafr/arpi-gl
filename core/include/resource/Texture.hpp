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


/*
 * Texture.h
 *
 *  Created on: 26 mai 2015
 *      Author: excilys
 */

#ifndef _DMA_TEXTURE_H_
#define _DMA_TEXTURE_H_

#include "utils/GLES2Logger.hpp"

#include <string>

#include "common/Types.hpp"
#include "resource/Image.hpp"
#include "GpuResource.hpp"

namespace dma {

    /**
     * Stores information relative to a GL Texture (name, gl handle, etc...) .
     */
    class Texture : public GpuResource {

        friend class TextureManager;

    public:
        Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        inline GLuint getHandle() const {
            return mHandle;
        }

        inline const std::string& getSID() const {
            return mSID;
        }

        inline void setSID(const std::string& sid) {
            mSID = sid;
        }

        virtual void load(const std::string& filename) = 0;
        virtual void refresh(const std::string& filename) = 0;
        virtual void wipe() override;

    protected:
        // openGL handle to this texture.
        GLuint mHandle;
        std::string mSID;

    };

} /* namespace dma */

#endif /* _DMA_TEXTURE_H_ */
