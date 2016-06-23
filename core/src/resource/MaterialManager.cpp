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



#include "resource/MaterialManager.hpp"
#include "utils/ExceptionHandler.hpp"
#include "utils/MaterialReader.hpp"

constexpr auto TAG = "MaterialManager";

namespace dma {

    MaterialManager::MaterialManager(const std::string &localDir,
                                     ShaderManager& shaderManager,
                                     MapManager& mapManager) :
            ResourceManagerHandler(localDir),
            mShaderManager(shaderManager),
            mMapManager(mapManager)
    {}


    bool MaterialManager::hasResource(const std::string & sid) const {
        const std::string& path = mLocalDir + sid;
        Log::trace(TAG, "Checking if material %s.json exists", path.c_str());
        return Utils::fileExists(path + ".json") || Utils::fileExists(path + ".JSON");
    }


    void MaterialManager::load(std::shared_ptr<Material> material, const std::string& sid) {

        Log::trace(TAG, "Loading material %s ...", sid.c_str());

        std::string path = mLocalDir + "/" + sid + ".json";

        MaterialReader materialReader(path);
        Status status = materialReader.parse();
        if(status != STATUS_OK) {
            std::string error = "Error while parsing material " + path;
            Log::error(TAG, error.c_str());
            assert(!"Error while parsing material");
            throw std::runtime_error(error);
        }

        material->mSID = sid;


        /////////////////////////////////////////////////////////////////////////
        // Get the rendering order
        material->mBackToFront = materialReader.isBackToFront();

        material->mPassCount = 0;
        /////////////////////////////////////////////////////////////////////////
        // Iterate over the passes.
        while (materialReader.nextPass()) {

            Pass pass = Pass();

            /////////////////////////////////////////////////////////////////////////
            // 1. Get the cull mode if any
            if (materialReader.hasCullMode()) {
                std::string cullMode = materialReader.getCullMode();
                if (cullMode == "front") {
                    pass.mCullMode = Pass::CullMode::FRONT;
                }
                else if (cullMode == "back") {
                    pass.mCullMode = Pass::CullMode::BACK;
                }
                else if (cullMode == "none") {
                    pass.mCullMode = Pass::CullMode::NONE;
                }
                else {
                    Log::error(TAG, "Invalid cull mode while parsing material %s", path.c_str());
                    assert(!"Invalid cull mode while parsing material");
                }
            }

            /////////////////////////////////////////////////////////////////////////
            // 2. Get the depth writing
            pass.mDepthWriting = materialReader.hasDepthWriting();

            /////////////////////////////////////////////////////////////////////////
            // 3. Get the shader
            std::string shaderSID = materialReader.getShader();
            if (shaderSID.empty()) {
                std::string error = "Invalid shader while parsing material " + path;
                Log::error(TAG, error.c_str());
                throw std::runtime_error(error);
            } else {
                pass.setShaderProgram(mShaderManager.acquire(shaderSID));
            }

            /////////////////////////////////////////////////////////////////////////
            // 4. Get the diffuse map if any
            if (materialReader.hasDiffuseMap()) {
                std::string diffuseSID = materialReader.getDiffuseMap();
                std::shared_ptr<Map> diffuseMap = mMapManager.acquire(diffuseSID);
                if (!pass.getShaderProgram()->hasUniform(ShaderProgram::UniformSem::DM)) {
                    std::string error = "Shader " + path + "doesn't have diffuseMap uniform";
                    Log::error(TAG, error.c_str());
                    throw std::runtime_error(error);
                }

                pass.setDiffuseMap(diffuseMap);
                pass.addFunc(Pass::Func::DIFFUSE_MAP);

                //Check if material can enable/disable the diffuse map
                if (materialReader.hasDiffuseMapActivation()) {
                    if (!pass.getShaderProgram()->hasUniform(ShaderProgram::UniformSem::DM_ACTIVATION)) {
                        std::string error = "Shader " + path + "doesn't have diffuse map activation uniform";
                        Log::error(TAG, error.c_str());
                        throw std::runtime_error(error);
                    }
                    pass.addFunc(Pass::Func::DIFFUSE_MAP_ACTIVATION);
                }
            }

            /////////////////////////////////////////////////////////////////////////
            // 5. Enable lighting computation if needed
            if (materialReader.hasLighting()) {
                if (!pass.getShaderProgram()->hasAttribute(ShaderProgram::AttribSem::NORMAL)) {
                    std::string error = "Shader " + path + "doesn't have normal attribute";
                    Log::error(TAG, error.c_str());
                    throw std::runtime_error(error);
                }
                if (!pass.getShaderProgram()->hasUniform(ShaderProgram::UniformSem::N)) {
                    std::string error = "Shader " + path + "doesn't have N uniform";
                    Log::error(TAG, error.c_str());
                    throw std::runtime_error(error);
                }
                // Enable flat or smooth shading
                std::string lightingMode = materialReader.getLighting();
                if (lightingMode == "flat") {
                    pass.addFunc(Pass::Func::LIGHTING_FLAT);
                } else if (lightingMode == "smooth") {
                    pass.addFunc(Pass::Func::LIGHTING_SMOOTH);
                } else {
                    std::string error = "Shader " + path + "has bad lighting mode";
                    Log::error(TAG, error.c_str());
                    throw std::runtime_error(error);
                }
            }

            /////////////////////////////////////////////////////////////////////////
            // 6. Enable scaling if needed
            if (materialReader.hasScaling()) {
                pass.addFunc(Pass::Func::SCALING);
            }

            /////////////////////////////////////////////////////////////////////////
            // 7. Get the diffuse color if any
            if (materialReader.hasDiffuseColor()) {
                if (!pass.getShaderProgram()->hasUniform(ShaderProgram::UniformSem::DIFFUSE_COLOR)) {
                    std::string error = "Shader " + path + "doesn't have color uniform";
                    Log::error(TAG, error.c_str());
                    throw std::runtime_error(error);
                }
                glm::vec3 diffuseColor = materialReader.getDiffuseColor();
                pass.setDiffuseColor(diffuseColor);
                pass.addFunc(Pass::Func::DIFFUSE_COLOR);
            }

            material->addPass(pass);
        }
        Log::trace(TAG, "Material %s loaded", sid.c_str());
    }

    std::shared_ptr<Material> MaterialManager::create() {
        return std::make_shared<Material>();
    }

    std::shared_ptr<Material> MaterialManager::create(const std::string &sid) {
        return std::make_shared<Material>(*acquire(sid));
    }
}
