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

    const std::string MaterialManager::FALLBACK_MATERIAL_SID = "fallback";


    /*====================== PUBLIC =======================*/

    //------------------------------------------------------------------------------
    //DESTRUCTOR
    MaterialManager::~MaterialManager() {
    }

    //METHODS


    //------------------------------------------------------------------------------
    Status MaterialManager::init() {
        Status result;
        mFallbackMaterial = std::make_shared<Material>();
        result = mLoad(mFallbackMaterial, FALLBACK_MATERIAL_SID);
        assert(result == STATUS_OK);
        return result;
    }

    //------------------------------------------------------------------------------
    Status MaterialManager::reload() {
        Log::trace(TAG, "Reloading MaterialManager...");

        mFallbackMaterial->reset();
        mLoad(mFallbackMaterial, FALLBACK_MATERIAL_SID);

        for (auto& kv : mMaterials) {
            const std::string& sid = kv.first;
            auto material = kv.second;
            if (material != nullptr) {
                material->reset();
                if (mLoad(material, sid) != STATUS_OK) {
                    Log::error(TAG, "Error while reloading material %s", sid.c_str());
                    return STATUS_KO;
                }
            }
        }

        Log::trace(TAG, "MaterialManager reloaded");
        return STATUS_OK;
    }


    //------------------------------------------------------------------------------
    std::shared_ptr<Material> MaterialManager::acquire(const std::string& sid) {
        // Check if material doesn't exist
        if (mMaterials.find(sid) == mMaterials.end()) {
            std::shared_ptr<Material> material = std::make_shared<Material>();
            if (mLoad(material, sid) != STATUS_OK) {
                Log::warn(TAG, "Material %s doesn't exist, returning fallback instead", sid.c_str());
                return mFallbackMaterial;
            }
            mMaterials[sid] = material;
        }
        return mMaterials[sid];
    }


    //------------------------------------------------------------------------------
    void MaterialManager::unload() {
        Log::trace(TAG, "Unloading MaterialManager...");
        if (mFallbackMaterial != nullptr) {
            mFallbackMaterial = nullptr; //release reference count
        }
        mMaterials.clear();
        Log::trace(TAG, "MaterialManager unloaded...");
    }


    //------------------------------------------------------------------------------
    bool MaterialManager::hasResource(const std::string & sid) const {
        const std::string& path = mLocalDir + sid;
        Log::trace(TAG, "Checking if material %s.json exists", path.c_str());
        return Utils::fileExists(path + ".json") || Utils::fileExists(path + ".JSON");
    }


    /*====================== PRIVATE =======================*/

    //CONSTRUCTORS
    //------------------------------------------------------------------------------
    MaterialManager::MaterialManager(const std::string &localDir,
                                     ShaderManager& shaderManager,
                                     MapManager&mapManager) :
            mLocalDir(localDir),
            mShaderManager(shaderManager),
            mMapManager(mapManager)
    {
    }


    //METHODS

    //------------------------------------------------------------------------------
    Status MaterialManager::mLoad(std::shared_ptr<Material> material, const std::string& sid) const {

        Log::trace(TAG, "Loading material %s ...", sid.c_str());

        std::string path = mLocalDir + sid + ".json";

        MaterialReader materialReader(path);
        Status status = materialReader.parse();
        if(status != STATUS_OK) {
            Log::error(TAG, "Error while parsing material %s", path.c_str());
            assert(!"Error while parsing material");
            return status;
        }

        material->mSID = sid;

        // from now, an error will be because of an invalid file.
        const ExceptionType badFileException = ExceptionType::INVALID_FILE;


        /////////////////////////////////////////////////////////////////////////
        // Get the rendering order
        material->mBackToFront = materialReader.isBackToFront();

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
                Log::error(TAG, "Invalid shader while parsing material %s", path.c_str());
                assert(!"Invalid shader while parsing material");
                return throwException(TAG, badFileException, "Invalid shader while parsing material " + path);
            } else {
                pass.setShaderProgram(mShaderManager.acquire(shaderSID));
            }

            /////////////////////////////////////////////////////////////////////////
            // 4. Get the diffuse map if any
            if (materialReader.hasDiffuseMap()) {
                std::string diffuseSID = materialReader.getDiffuseMap();
                std::shared_ptr<Map> diffuseMap = mMapManager.acquire(diffuseSID);
                if (!pass.getShaderProgram()->hasUniform(ShaderProgram::UniformSem::DM)) {
                    Log::error(TAG, "Error while parsing %s: "
                            "Shader doesn't have diffuseMap uniform", path.c_str());
                    assert(!"Shader doesn't have diffuseMap uniform");
                    return throwException(TAG, badFileException, "Error while parsing " + path + " : Shader doesn't have diffuseMap uniform");
                }

                pass.setDiffuseMap(diffuseMap);
                pass.addFunc(Pass::Func::DIFFUSE_MAP);

                //Check if material can enable/disable the diffuse map
                if (materialReader.hasDiffuseMapActivation()) {
                    if (!pass.getShaderProgram()->hasUniform(ShaderProgram::UniformSem::DM_ACTIVATION)) {
                        Log::error(TAG, "Error while parsing %s: "
                                "Shader doesn't have diffuse map activation uniform", path.c_str());
                        assert(!"Shader doesn't have diffuse map activation uniform");
                        return throwException(TAG, badFileException, "Error while parsing " + path + " : Shader doesn't have diffuse map activation uniform");
                    }
                    pass.addFunc(Pass::Func::DIFFUSE_MAP_ACTIVATION);
                }
            }

            /////////////////////////////////////////////////////////////////////////
            // 5. Enable lighting computation if needed
            if (materialReader.hasLighting()) {
                if (!pass.getShaderProgram()->hasAttribute(ShaderProgram::AttribSem::NORMAL)) {
                    Log::error(TAG, "Error while parsing %s: "
                            "Shader doesn't have normal attribute", path.c_str());
                    assert(!"Shader doesn't have normal attribute");
                    return throwException(TAG, badFileException, "Error while parsing " + path + " : Shader doesn't have normal attribute");
                }
                if (!pass.getShaderProgram()->hasUniform(ShaderProgram::UniformSem::N)) {
                    Log::error(TAG, "Error while parsing %s: "
                            "Shader doesn't have N uniform", path.c_str());
                    assert(!"Shader doesn't have N uniform");
                    return throwException(TAG, badFileException, "Error while parsing " + path + " : Shader doesn't have N uniform");
                }
                // Enable flat or smooth shading
                std::string lightingMode = materialReader.getLighting();
                if (lightingMode == "flat") {
                    pass.addFunc(Pass::Func::LIGHTING_FLAT);
                } else if (lightingMode == "smooth") {
                    pass.addFunc(Pass::Func::LIGHTING_SMOOTH);
                } else {
                    Log::error(TAG, "bad lighting mode");
                    assert(!"bad lighting mode");
                    return throwException(TAG, badFileException, "Error while parsing " + path + " : bad lighting mode");
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
                    Log::error(TAG, "Error while parsing %s: "
                            "Shader doesn't have diffuse color uniform", path.c_str());
                    assert(!"Shader doesn't have diffuse color uniform");
                    return throwException(TAG, badFileException, "Error while parsing " + path + " : Shader doesn't have color uniform");
                }
                glm::vec3 diffuseColor = materialReader.getDiffuseColor();
                pass.setDiffuseColor(diffuseColor);
                pass.addFunc(Pass::Func::DIFFUSE_COLOR);
            }

            material->addPass(pass);
        }
        Log::trace(TAG, "Material %s loaded", sid.c_str());
        return STATUS_OK;
    }



    //------------------------------------------------------------------------------
    std::shared_ptr<Material> MaterialManager::create() {
        return std::make_shared<Material>();
    }


    //------------------------------------------------------------------------------
    std::shared_ptr<Material> MaterialManager::create(const std::string &sid) {
        return std::make_shared<Material>(*acquire(sid));
    }


    //------------------------------------------------------------------------------
    void MaterialManager::update() {
        auto it = mMaterials.begin();
        while (it != mMaterials.end()) {
            if (it->second.unique()) {
                it = mMaterials.erase(it);
            } else {
                ++it;
            }
        }
    }
}
