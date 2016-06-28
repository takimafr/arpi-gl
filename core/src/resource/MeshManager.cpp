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



#include "resource/MeshManager.hpp"
#include "utils/ObjReader.hpp"

#include <algorithm>
#include <string.h>

constexpr auto TAG = "MeshManager";

namespace dma {

    /* ================= ROUTINES ========================*/


    Status loadObj(const std::string& path,
                   std::vector<glm::vec3>& positions,
                   std::vector<glm::vec2>& uvs,
                   std::vector<glm::vec3>& flatNormals,
                   std::vector<VertexIndices>& vertexIndices) {
        /////////////////////////////////////////////////////
        //1. open obj file.
        ObjReader objReader(path);
        if(!objReader.isOpen()) {
            Log::error(TAG, "Cannot open file %s", path.c_str());
            assert(!"Cannot open obj file");
            return STATUS_KO;
        }

        /////////////////////////////////////////////////////
        //2. parse positions
        glm::vec3 p;
        objReader.gotoPositions();
        while(objReader.nextPosition(p)) {
            positions.push_back(p);
        }

        if(positions.empty()) {
            Log::error(TAG, "No position vertex found while reading file \"%s\"", path.c_str());
            assert(!"No position vertex found");
            return STATUS_KO;
        }
        /////////////////////////////////////////////////////
        //3. parse uvs
        glm::vec2 uv;
        objReader.gotoUV();
        while(objReader.nextUV(uv)) {
            uvs.push_back(uv);
        }

        /////////////////////////////////////////////////////
        // 4. parse normals as flat normal
        glm::vec3 n;
        objReader.gotoNormals();
        while(objReader.nextNormal(n)) {
            flatNormals.push_back(n);
        }

        /////////////////////////////////////////////////////
        // 5. parse faces
        U16 f[3][3];
        objReader.gotoFaces();
        while (objReader.nextFace(f)) {
            for (int i = 0; i < 3; ++i) {
                VertexIndices vi(f[i][ObjReader::FACE_P],
                                 f[i][ObjReader::FACE_UV],
                                 f[i][ObjReader::FACE_N]);

                vertexIndices.push_back(vi);
            }
        }
        return STATUS_OK;
    }



    inline bool containsNormal(const std::vector<glm::vec3> &normals,
                               const glm::vec3& n) {
        F32 e = 0.0001f;
        for (glm::vec3 normal : normals) {
            if(glm::abs(glm::dot<float>(normal, n) - 1) < e) return true;
        }
        return false;
    }



    void generateFlatNormals(std::vector<glm::vec3>& flatNormals,
                             const std::vector<glm::vec3>& positions,
                             std::vector<VertexIndices>& indices) {

        U16 ifn = 0;
        for(U16 i = 0; i < indices.size(); i += 3) {
            U16 ia = indices[i].p;
            U16 ib = indices[i+1].p;
            U16 ic = indices[i+2].p;

            glm::vec3 normal = glm::normalize(glm::cross(
                    positions[ib] - positions[ia],
                    positions[ic] - positions[ia]));
            flatNormals.push_back(normal);
            indices[i].fn = indices[i+1].fn = indices[i+2].fn = ifn;
            ifn +=1;
        }
    }



    void generateSmoothNormals(std::vector<glm::vec3>& smoothNormals,
                               const std::vector<glm::vec3>& flatNormals,
                               const std::vector<glm::vec3>& positions,
                               std::vector<VertexIndices>& indices,
                               bool keepFlats = false) {

        std::vector<std::vector<glm::vec3>> smoothBuckets(positions.size());
        for (U16 i = 0; i < indices.size(); ++i) {
            glm::vec3 normal = flatNormals[indices[i].fn];
            U16 isn = indices[i].p;
            if (!containsNormal(smoothBuckets[isn], normal)) {
                smoothBuckets[isn].push_back(normal);
            }
            indices[i].sn = isn;
        }

        for (std::vector<glm::vec3> sb : smoothBuckets) {
            glm::vec3 sn = glm::vec3(0.0f);
            for(glm::vec3 n : sb) {
                sn += n;
            }
            sn = glm::normalize(sn / (float)sb.size());
            smoothNormals.push_back(sn);
        }

        if (!keepFlats) {
            for(VertexIndices& vi : indices) {
                vi.fn = 0xFFFF;
            }
        }
    }



    BoundingSphere generateBoundingSphere(const std::vector<glm::vec3>& positions) {
        glm::vec3 center(0.0f, 0.0f, 0.0f);
        float radius = 0.0f;

        for (const glm::vec3& p : positions) {
            center += p;
        }
        center /= positions.size();

        for (const glm::vec3& p : positions) {
            float d = glm::distance(center, p);
            radius = glm::max(radius, d);
        }
        return BoundingSphere(center, radius);
    }


    /* ================= MEMBERS ========================*/


    MeshManager::MeshManager(const std::string& localDir) :
            GpuResourceManagerHandler(localDir)
    {}

    MeshManager::~MeshManager() {
    }

    std::shared_ptr<Mesh> MeshManager::create(std::vector<glm::vec3> &positions,
                                              std::vector<glm::vec2> &uvs,
                                              std::vector<glm::vec3> &flatNormals,
                                              std::vector<glm::vec3> &smoothNormals,
                                              std::vector<VertexIndices> &indices) {
        // Load the anonymous mesh
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
        load(mesh, "_anonymous_", positions, uvs, flatNormals, smoothNormals, indices);
        mAnonymousResources.push_back(mesh);
        return mesh;
    }


    bool MeshManager::hasResource(const std::string & sid) const {
        //filename, deduced from SID
        const std::string& path = mLocalDir + sid;
        Log::trace(TAG, "checking if mesh %s exists...", sid.c_str());
        return Utils::fileExists(path + ".obj") || Utils::fileExists(path + ".OBJ");
    }


    void MeshManager::load(std::shared_ptr<Mesh> mesh, const std::string& sid) {
        //try to load from the cache
        if (mesh->hasCache()) {
            return load(mesh, sid,
                        mesh->positions,
                        mesh->uvs,
                        mesh->flatNormals,
                        mesh->smoothNormals,
                        mesh->vertexIndices);
        }

        //otherwise load from the file

        // stores elements as they come from .obj
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> flatNormals;
        std::vector<glm::vec3> smoothNormals;
        // map one UV & one p per vertex object
        std::vector<VertexIndices> vertexIndices;

        //filename, deduced from SID
        std::string path = mLocalDir + "/" + sid + ".obj";

        //load positions uvs and their indices from the obj file
        if (loadObj(path, positions, uvs, flatNormals, vertexIndices) != STATUS_OK) {
            std::string error = "Unable to load obj " + sid;
            Log::error(TAG, error.c_str());
            throw std::runtime_error(error);
        }

        load(mesh, sid, positions, uvs, flatNormals, smoothNormals, vertexIndices);
    }



    void MeshManager::load(std::shared_ptr<Mesh> mesh, const std::string &sid,
                           std::vector<glm::vec3> &positions,
                           std::vector<glm::vec2> &uvs,
                           std::vector<glm::vec3> &flatNormals,
                           std::vector<glm::vec3> &smoothNormals,
                           std::vector<VertexIndices> &vertexIndices) const {

        bool hasUv, hasFlat, hasSmooth;
        hasUv = !uvs.empty();
        if (!hasUv) {
            Log::trace(TAG, "no UV mapping found for mesh \"%s\"", sid.c_str());
        }

        hasFlat = !flatNormals.empty();
        if (!hasFlat) {
            Log::trace(TAG, "no Flat normal mapping found for mesh \"%s\"", sid.c_str());
        }

        // generates normals
        if (!hasFlat) {
            generateFlatNormals(flatNormals, positions, vertexIndices);
            hasFlat = true; //TODO remove: used for building poc
        }

        hasSmooth = !smoothNormals.empty();
        if (!hasSmooth) {
            Log::trace(TAG, "no Smooth normal mapping found for mesh \"%s\"", sid.c_str());
        }

        // generates smooth
        if (!hasSmooth) {
            generateSmoothNormals(smoothNormals, flatNormals, positions, vertexIndices, hasFlat);
            hasSmooth = true; //TODO metadata
        }

        if(!hasFlat) { //TODO handle it with metadata
            flatNormals.clear(); //no need from there
        }

        std::vector<U16> indices;
        std::vector<Vertex> vertices;
        // map one Vertex object to one or many VertexIndices
        std::map<VertexIndices, U16> indexMap;

        U16 currentVertexIndex = 0;
        //create Vertex objects out of VertexIndices.
        for (const VertexIndices& vi : vertexIndices) {

            // if Vertex object of this indices doesn't exist already
            if (indexMap.find(vi) == indexMap.end()) {
                //create it & refer to it.
                indexMap[vi] = currentVertexIndex;
                indices.push_back(currentVertexIndex);
                currentVertexIndex++;

                Vertex v;
                v.setPosition(positions[vi.p]);
                if (hasUv) {
                    v.setUv(uvs[vi.uv]);
                }
                if (hasFlat) {
                    v.setFlatNormal(flatNormals[vi.fn]);
                }
                if (hasSmooth) {
                    v.setSmoothNormal(smoothNormals[vi.sn]);
                }
                vertices.push_back(v);
            } else {
                indices.push_back(indexMap[vi]);
            }
        }

        U32 vertexSize = 0;
        U32 vertexCount = (U32) vertices.size();

        //Positions
        VertexElement positionElement(VertexElement::Semantic::POSITION, 3, GL_FLOAT, vertexSize);
        vertexSize += positionElement.getSizeInByte();
        mesh->addVertexElement(positionElement);


        //Normals
        if (hasFlat) {
            VertexElement flatNormalElement(VertexElement::Semantic::FLAT_NORMAL, 3, GL_FLOAT, vertexSize);
            vertexSize += flatNormalElement.getSizeInByte();
            mesh->addVertexElement(flatNormalElement);
        }
        if (hasSmooth) {
            VertexElement smoothNormalElement(VertexElement::Semantic::SMOOTH_NORMAL, 3, GL_FLOAT, vertexSize);
            vertexSize += smoothNormalElement.getSizeInByte();
            mesh->addVertexElement(smoothNormalElement);
        }

        // UVs
        if (hasUv) {
            VertexElement uvElement(VertexElement::Semantic::UV, 2, GL_FLOAT, vertexSize);
            vertexSize += uvElement.getSizeInByte();
            mesh->addVertexElement(uvElement);
        }

        mesh->mVertexSize = vertexSize;
        mesh->mVertexCount = vertexCount;
        //Log::debug(TAG, "vertexSize=%d vertexCount=%d", vertexSize, vertexCount);
        //Log::debug(TAG, "indices=%d", indices.size());

        BYTE* data = new BYTE[vertexSize * vertexCount];

        /////////////////////////////////////////////////////////////////////////
        // Generate vertex buffer
        if (mesh->mVertexBuffer != nullptr) {
            mesh->mVertexBuffer->wipe();
        }
        mesh->mVertexBuffer = std::make_shared<VertexBuffer>(vertexSize, (U32) (vertexSize * vertexCount));

        /////////////////////////////////////////////////////////////////////////
        // Fills data
        for (U32 v = 0; v < vertexCount; ++v) {
            if (mesh->hasVertexElement(VertexElement::Semantic::POSITION)) {
                VertexElement ve = mesh->getVertexElement(VertexElement::Semantic::POSITION);
                memcpy(&data[v * vertexSize + ve.getOffset()], &(vertices[v].getPosition()), ve.getSizeInByte());
            }
            if (mesh->hasVertexElement(VertexElement::Semantic::FLAT_NORMAL)) {
                VertexElement ve = mesh->getVertexElement(VertexElement::Semantic::FLAT_NORMAL);
                memcpy(&data[v * vertexSize + ve.getOffset()], &(vertices[v].getFlatNormal()), ve.getSizeInByte());
            }
            if (mesh->hasVertexElement(VertexElement::Semantic::SMOOTH_NORMAL)) {
                VertexElement ve = mesh->getVertexElement(VertexElement::Semantic::SMOOTH_NORMAL);
                memcpy(&data[v * vertexSize + ve.getOffset()], &(vertices[v].getSmoothNormal()), ve.getSizeInByte());
            }
            if (mesh->hasVertexElement(VertexElement::Semantic::UV)) {
                VertexElement ve = mesh->getVertexElement(VertexElement::Semantic::UV);
                memcpy(&data[v * vertexSize + ve.getOffset()], &(vertices[v].getUv()), ve.getSizeInByte());
            }
        }

        /////////////////////////////////////////////////////////////////////////
        // Uploads data to GPU
        mesh->mVertexBuffer->writeData(0, vertexSize * vertexCount, data);
        delete[] data;


        if (mesh->mIndexBuffer != nullptr) {
            mesh->mIndexBuffer->wipe();
        }
        mesh->mIndexBuffer = std::make_shared<IndexBuffer>((U32) indices.size());
        mesh->mIndexBuffer->writeData(indices.data());

        mesh->mBoundingSphere = generateBoundingSphere(positions);

        //update the cache
        mesh->positions = positions;
        mesh->uvs = uvs;
        mesh->flatNormals = flatNormals;
        mesh->smoothNormals = smoothNormals;
        mesh->vertexIndices = vertexIndices;

        mesh->mSID = sid;

        Log::trace(TAG, "Mesh %s loaded", sid.c_str());
    }
}
