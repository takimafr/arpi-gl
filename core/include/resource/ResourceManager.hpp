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


#ifndef _DMA_RESOURCE_MANAGER_HPP_
#define _DMA_RESOURCE_MANAGER_HPP_

#include "resource/IResourceManager.hpp"
#include "resource/CubeMapManager.hpp"
#include "resource/ShaderManager.hpp"
#include "resource/MeshManager.hpp"
#include "resource/MapManager.hpp"
#include "resource/MaterialManager.hpp"
#include "resource/QuadFactory.hpp"

#include <string>

namespace dma {

    /**
     * This class takes care of the resource manager life cycle and is a facade to their methods
     */
    class ResourceManager {

        /* ***
         * FRIEND CLASS
         */
        friend class Engine;

        /* ***
         * CONSTANTS
         */
        /** base dir where resources are stored, by type. Defined in cpp implementation. */
        static const char* RESOURCE_PATHS[];
        static constexpr char TAG[] = "ResourceManager";

    public:
        /* ***
         * ENUM
         */
        enum ResourceType {
            SHADER = 0, MESH = 1, TEXTURE = 2, MATERIAL = 3, SCENE = 4, CUBEMAP = 5, TILE = 6, size = 7
        };

        struct ResourceIds {
            struct CubeMap {
                static constexpr char DEFAULT[]      = "default";
            };

        public:
            struct Texture {
                static constexpr char DEFAULT[]      = "damier";
                static constexpr char FALLBACK[]     = "fallback";
            };

            struct Mesh {
                static constexpr char BALOON[]  = "balloon";
                static constexpr char CUBE[]    = "cube";
                static constexpr char PYRAMID[] = "pyramid";
                static constexpr char SPHERE[]  = "sphere";
            };

            struct Shader {
                static constexpr char FALLBACK[]  = "fallback";
            };

        };

        /* ***
         * CONSTRUCTOR & DESTRUCTOR
         */
        virtual ~ResourceManager();


        Status init();

        /**
         * Reloads all current resources from disk
         */
        Status reload();
        /**
         * Refreshes current resources from cache if any and all OpenGL resources according to the current context
         */
        Status refresh();

        /**
         * Clean all GPU resources
         */
        void wipe();

        /**
         * Free memory used by this resource manager.
         */
        void unload();


    private:
        /**
         * Create a new resourceManager that will use the provided path as resource dir.
         * This constructor will create the required ShaderManager, MeshManager & TextureManager.
         * @param std::string
         *              path to resource dir.
         */
        ResourceManager(const std::string&);
        ResourceManager(const ResourceManager &) = delete;
        void operator=(const ResourceManager &) = delete;

    public:

        /**
         * @return true if the corresponding shader program exists.
         */
        inline bool hasShaderProgram(const std::string& sid) const {
            return mShaderManager.hasResource(sid);
        }


        /**
         * @return the ShaderProgram corresponding to the sid
         * @param const std::string& -
         *          SID of the shader to load.
         * @param Status* -
         *          holds Status::OK if the mesh could be loaded.
         */
        inline std::shared_ptr<ShaderProgram> acquireShaderProgram(const std::string& sid) {
            return mShaderManager.acquire(sid);
        }


        /**
         * @return true if the corresponding mesh exists.
         */
        inline bool hasMesh(const std::string& sid) const {
            return mMeshManager.hasResource(sid);
        }


        /**
         * @param const std::string& -
         *          SID of the mesh to load.
         * @param Status* -
         *          holds Status::OK if the mesh could be loaded.
         * @return the Mesh corresponding to the sid
         */
        inline std::shared_ptr<Mesh> acquireMesh(const std::string& sid) {
            return mMeshManager.acquire(sid);
        }



        /**
         * @return true if the corresponding map program exists.
         */
        inline bool hasMap(const std::string& sid) const {
            return mMapManager.hasResource(sid);
        }



        /**
         * @param const std::string&
         *                  SID of the texture.
         *                  If no SID provided, will acquire the default fallback texture.
        * @param Status* -
         *          holds Status::OK if the texture could be loaded.
         * @return the Texture corresponding to the sid
         */
        inline std::shared_ptr<Map> acquireMap(const std::string &sid) {
            return mMapManager.acquire(sid);
        }



        /**
         * @param const std::string&
         *                  SID of the texture.
         *                  If no SID provided, will acquire the default fallback texture.
         * @param Status* -
         *          holds Status::OK if the cubemap could be loaded.
         * @return the Texture corresponding to the sid
         */
        inline std::shared_ptr<CubeMap> acquireCubeMap(const std::string &sid) {
            return mCubeMapManager.acquire(sid);
        }



        /**
         * @return true if the corresponding material exists.
         */
        inline bool hasMaterial(const std::string& sid) const {
            return mMaterialManager.hasResource(sid);
        }



        /**
         * @param const std::string& -
         *              the SID of the material.
         * @param bool* -
         *              holds true if material could be created.
         * @return the Material corresponding to the sid
         */
        inline std::shared_ptr<Material> acquireMaterial(const std::string& sid) {
            return mMaterialManager.acquire(sid);
        }



        /**
         * Creates a new empty Material
         */
        inline std::shared_ptr<Material> createMaterial() {
            return mMaterialManager.create();
        }



        /**
         * Creates a new Material corresponding to the sid
         */
        inline std::shared_ptr<Material> createMaterial(const std::string& sid) {
            return mMaterialManager.create(sid);
        }



        /**
         * Creates a new quad width * height
         * @note You must not delete the quad but instead call the deleteQuad method
         * @param width the width of the quad
         * @param height the height of the quad
         */
        inline std::shared_ptr<Quad> createQuad(F32 width, F32 height) {
            return mQuadFactory.createQuad(width, height);
        }


        /**
         * Updates all resources: ie unload unused resources
         */
        void update();

    private:
        /* ***
         * ATTRIBUTES
         */

        std::string                mResourceDir;

        ShaderManager              mShaderManager;
    public: MeshManager                mMeshManager; //TODO remove public: used for building/tracks POC
        MapManager                 mMapManager;
        CubeMapManager             mCubeMapManager;
        MaterialManager            mMaterialManager;
        QuadFactory                mQuadFactory;

        static constexpr int RESOURCE_MANAGER_ARRAY_SIZE = 6;
        /** array of the above resource managers. */
        //IResourceManager<void>*    mResourceManagers[RESOURCE_MANAGER_ARRAY_SIZE];
    };

}



#endif /* _DMA_RESOURCE_MANAGER_HPP_ */
