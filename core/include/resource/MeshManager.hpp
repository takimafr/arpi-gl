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


#ifndef _DMA_MESHMANAGER_HPP_
#define _DMA_MESHMANAGER_HPP_

#include <string>
#include <map>
#include <vector>
#include <rendering/Vertex.hpp>
#include <utils/GLES2Logger.hpp>

#include "resource/IResourceManager.hpp"
#include "utils/VertexIndices.hpp"
#include "resource/Mesh.hpp"
#include "glm/glm.hpp"



namespace dma {

    class MeshManager : public IResourceManager<Mesh> {

        friend class ResourceManager;

        static const std::string FALLBACK_MESH_SID;


    public:
        virtual ~MeshManager();

        /**
         * Load the fallback mesh
         */
        Status init();

        /**
         * @param const std::string& -
         *          SID of the mesh to load.
         * @param Status* -
         *          holds Status::OK if the mesh could be loaded.
         * @return the Mesh corresponding to the sid.
         */
        std::shared_ptr<Mesh> acquire(const std::string& sid, Status* result);

        /**
         * From disk
         */
        Status reload();

        void update() override;

        /**
         * From cache if any
         */
        Status refresh();

        /**
         * Clean all GPU resources
         */
        void wipe();


        /**
         * Clear all resources used by the meshes but keep the pointer to them.
         * The mesh pointer remains valid, but the mesh itself is not.
         * This is typically used when you need to refresh the resources
         * for example when the OpenGL context has changed.
         */
        void unload();

        bool hasResource(const std::string &) const;

    private:
        MeshManager(const std::string& rootDir);
        MeshManager(const MeshManager&) = delete;
        void operator=(const MeshManager&) = delete;

        //METHODS
        //Mesh* mLoad(const std::string& sid, bool* result) const;
        //Mesh* mLoad(Mesh* mesh, const std::string& sid, bool* result) const;
        Status mLoad(std::shared_ptr<Mesh> mesh, const std::string& sid) const;
        Status mLoad(std::shared_ptr<Mesh> mesh, const std::string& sid,
                     std::vector<glm::vec3> &positions,
                     std::vector<glm::vec2>& uvs,
                     std::vector<glm::vec3>& flatNormals,
                     std::vector<VertexIndices>& vertexIndices) const;

        // FIELDS
        std::map<std::string, std::shared_ptr<Mesh>> mMeshes;
        std::shared_ptr<Mesh> mFallbackMesh;
        std::string mLocalDir;
    };
}



#endif //_DMA_MESHMANAGER_HPP_
