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

#include "resource/ResourceManagerHandler.hpp"
#include "utils/VertexIndices.hpp"
#include "resource/Mesh.hpp"
#include "glm/glm.hpp"



namespace dma {

    class MeshManager : public ResourceManagerHandler<Mesh> {

        friend class ResourceManager;

    public:
        virtual ~MeshManager();

        /**
         * Load an anonymous mesh
         */
        std::shared_ptr<Mesh> load(std::vector<glm::vec3>& positions,
                                   std::vector<glm::vec2>& uvs,
                                   std::vector<glm::vec3>& flatNormals,
                                   std::vector<glm::vec3>& smoothNormals,
                                   std::vector<VertexIndices> &indices);

        /**
         * From disk
         */
        Status reload();

        /**
         * From cache if any
         */
        Status refresh();

        /**
         * Clean all GPU resources
         */
        void wipe();

        bool hasResource(const std::string &) const;

    private:
        MeshManager(const std::string& rootDir);
        MeshManager(const MeshManager&) = delete;
        void operator=(const MeshManager&) = delete;

        Status load(std::shared_ptr<Mesh> mesh, const std::string& sid) const;
        Status load(std::shared_ptr<Mesh> mesh, const std::string& sid,
                     std::vector<glm::vec3> &positions,
                     std::vector<glm::vec2>& uvs,
                     std::vector<glm::vec3>& flatNormals,
                     std::vector<glm::vec3>& smoothNormals,
                     std::vector<VertexIndices>& vertexIndices) const;

        std::string mLocalDir;
    };
}



#endif //_DMA_MESHMANAGER_HPP_
