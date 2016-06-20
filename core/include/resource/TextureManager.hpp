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


#ifndef _DMA_TEXTUREMANAGER_HPP_
#define _DMA_TEXTUREMANAGER_HPP_

#include <string>
#include <map>

#include "common/Types.hpp"
#include "resource/IResourceManager.hpp"
#include "resource/Texture.hpp"
#include "resource/Map.hpp"

namespace dma {

    class TextureManager : public IResourceManager<Texture> {

        /** this class is *only* intended to be used by ResourceManager. */
        friend class ResourceManager;

    protected:

        /* ***
         * CONSTRUCTORS
         */

        /**
         * creates a TextureManager with supplied path as rootDir.
         * @param std::string
         *          Path to the resource root directory.
         */
        TextureManager(const std::string & = "");
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        Status mLoadMap(std::shared_ptr<Map> map, const std::string &sid);

    public :

        virtual ~TextureManager();

        /* ***
         * PUBLIC METHODS
         */

        /**
         * Initializes resources, this method is called when EGL context is up
         */
        Status init();

        /**
         * @param const std::string&
         *                  SID of the texture.
         * @param Status* -
         *              holds Status::OK if the shader could be loaded.
         * @return the loaded shader, or default shader if none could be loaded.
         */
        virtual std::shared_ptr<Texture> acquire(const std::string&) override;


        //GLuint loadCubeMap(const std::string& sid);

        //bool refresh();

        virtual /**
         * From disk
         */
        Status reload();

        virtual /**
         * From cache
         */
        Status refresh();


        virtual /**
         * Clean all GPU resources
         */
        void wipe();

        virtual /**
         * Unload all the textures from the GPU and clean all resources.
         */
        void unload();

        virtual void update() override;

        virtual bool hasResource(const std::string &) const ;

        /* ***
         * PRIVATE METHODS
         */
        //const Texture* mAcquire(const std::string & sid, bool* result, bool isFallback);



        /* ***
         * PRIVATE ATTRIBUTES
         */
    protected :
        std::map<std::string, std::shared_ptr<Texture>> mTextures;
        std::shared_ptr<Map> mFallbackTexture;
        std::string mLocalDir;
        static const std::string FALLBACK_TEXTURE_SID;

    };

} /* namespace dma */

#endif /* _DMA_TEXTUREMANAGER_HPP_ */
