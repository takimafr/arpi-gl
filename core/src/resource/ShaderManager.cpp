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



#include "resource/ResourceManager.hpp"
#include "resource/ShaderManager.hpp"
#include "utils/ExceptionHandler.hpp"

constexpr auto TAG = "ShaderManager";

#define NAUTO_ADD_PRECISION
#ifdef AUTO_ADD_PRECISION
static const std::string FRAGMENT_SHADER_PRECISION_HEADER =
        "#ifdef GL_ES\n\
#ifdef GL_FRAGMENT_PRECISION_HIGH\n\
precision highp float;\n\
#else\n\
precision mediump float;\n\
#endif\n\
#endif\n";
#endif

namespace dma {
    const std::string ShaderManager::FALLBACK_SHADER_SID = "fallback";

    /* ================= ROUTINES ========================*/

    void printLog(GLuint shaderHandle) {
        GLint log_length = 0;
        GLboolean isShader = glIsShader(shaderHandle);
        if (isShader) {
            glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &log_length);
        } else {
            Log::error(TAG, "Shader %d is not a shader", shaderHandle);
            return;
        }

        char* log = new char[log_length];
        glGetShaderInfoLog(shaderHandle, log_length, NULL, log);
        Log::error(TAG, "----------------\n%s----------------------", log);
        delete[] log;
    }


    /* ================= PUBLIC ========================*/

    ShaderManager::~ShaderManager() {
//        Log::trace(TAG, "Destructing ShaderManager...");
//        unload();
//        Log::trace(TAG, "ShaderManager destructed");
    }



    Status ShaderManager::init() {
        Status result;
        mFallbackShaderProgram = std::make_shared<ShaderProgram>();
        result = mLoad(mFallbackShaderProgram, FALLBACK_SHADER_SID);
        assert(result == STATUS_OK);
        return result;
    }



    /*
     * Returns the pointer of the ShaderProgram with the String ID sid.
     * Increments the reference count
     */
    std::shared_ptr<ShaderProgram> ShaderManager::acquire(const std::string& sid) {
        if (sid == FALLBACK_SHADER_SID) {
            return mFallbackShaderProgram;
        }
        if (mShaderPrograms.find(sid) == mShaderPrograms.end()) {
            std::shared_ptr<ShaderProgram> shaderProgram = std::make_shared<ShaderProgram>();
            Status status = mLoad(shaderProgram, sid);
            if (status != STATUS_OK) {
                Log::warn(TAG, "cannot load ShaderProgram %s, returning fallback instead", sid.c_str());
                //clear the cache and delete shader
                return mFallbackShaderProgram;
            }
            mShaderPrograms[sid] = shaderProgram;
        }
        return mShaderPrograms[sid];
    }



    bool ShaderManager::hasResource(const std::string& sid) const {

        const std::string& path = mLocalDir + sid;
        bool res = Utils::fileExists(path + ".v.glsl") || Utils::fileExists(path + ".v.GLSL");
        res &=  (Utils::fileExists(path + ".f.glsl") || Utils::fileExists(path + ".f.glsl"));
        return res;
    }



    Status ShaderManager::reload() {
        Log::trace(TAG, "Reloading ShaderManager...");

        mFallbackShaderProgram->wipe();
        mFallbackShaderProgram->clearCache();
        mLoad(mFallbackShaderProgram, FALLBACK_SHADER_SID);

        for (auto& kv : mShaderPrograms) {
            const std::string& sid = kv.first;
            auto shaderProgram = kv.second;

            if (shaderProgram != nullptr) {
                shaderProgram->wipe();
                shaderProgram->clearCache();
                if (mLoad(shaderProgram, sid) != STATUS_OK) {
                    Log::error(TAG, "Error while reloading shader %s", sid.c_str());
                    assert(false);
                    return STATUS_KO;
                }
            }
        }

        Log::trace(TAG, "ShaderManager reloaded");
        return STATUS_OK;
    }



    Status ShaderManager::refresh() {
        Log::trace(TAG, "Refreshing ShaderManager...");

        //mFallbackShaderProgram->wipe();
        mLoad(mFallbackShaderProgram, FALLBACK_SHADER_SID);

        for (auto& kv : mShaderPrograms) {
            const std::string& sid = kv.first;
            auto shaderProgram = kv.second;
            //shaderProgram->wipe();
            if (mLoad(shaderProgram, sid) != STATUS_OK) {
                Log::error(TAG, "Error while reloading shader %s", sid.c_str());
                assert(false);
                return STATUS_KO;
            }
        }

        Log::trace(TAG, "ShaderManager refreshed");
        return STATUS_OK;
    }



    void ShaderManager::unload() {
        Log::trace(TAG, "Unloading ShaderManager...");

        mFallbackShaderProgram = nullptr; //release reference count
        mShaderPrograms.clear();

        Log::trace(TAG, "ShaderManager unloaded");
    }



    void ShaderManager::wipe() {
        Log::trace(TAG, "Wiping ShaderManager...");

        mFallbackShaderProgram->wipe();

        for (auto& kv : mShaderPrograms) {
            auto shaderProgram = kv.second;
            if (shaderProgram != nullptr) {
                shaderProgram->wipe();
            }
        }

        Log::trace(TAG, "ShaderManager wiped");
    }

    /* ================= PRIVATE ========================*/


    ShaderManager::ShaderManager(const std::string& localDir) :
            mShaderPrograms()
    {
        mLocalDir = localDir;
    }



    Status ShaderManager::mLoad(std::shared_ptr<ShaderProgram> shaderProgram, const std::string &sid) const {
        Log::trace(TAG, "Loading shader %s ...", sid.c_str());

        //try to load from the cache
        if (shaderProgram->hasCache()) {
            return mLoad(shaderProgram, sid, shaderProgram->mVertexSource, shaderProgram->mFragmentSource);
        }

        //otherwise load from the file

        std::string vertexSource;
        std::string fragmentSource;
        Status status;
        status = Utils::bufferize(mLocalDir + sid + ".v.glsl", vertexSource);
        if(status == STATUS_OK) {
            status = Utils::bufferize(mLocalDir + sid + ".f.glsl", fragmentSource);
        }

        if(status != STATUS_OK) {
            return status;
        }

        //update the cache
        shaderProgram->mVertexSource = vertexSource;
        shaderProgram->mFragmentSource = fragmentSource;

        return mLoad(shaderProgram, sid, vertexSource, fragmentSource);
    }



    Status ShaderManager::mLoad(std::shared_ptr<ShaderProgram> shaderProgram,
                                const std::string& sid,
                                const std::string &vertexSource,
                                const std::string &fragmentSource) const {
        // add precision directives in case it is missing.
#ifdef AUTO_ADD_PRECISION
        unsigned int found = fragmentSource.find_first_not_of(" \n");
        if (found!=std::string::npos) {
            if(fragmentSource[found] != '#' ) {
                fragmentSource = fragmentSource.insert(0, FRAGMENT_SHADER_PRECISION_HEADER);
            }
#endif
        GLuint vertexHandle = mCompile(vertexSource, GL_VERTEX_SHADER);
        GLuint fragmentHandle = mCompile(fragmentSource, GL_FRAGMENT_SHADER);

        if (vertexHandle == 0 || fragmentHandle == 0) {
            Log::error(TAG, "Unable to compile shader %s", sid.c_str());
            assert(!"cannot compile shader");
            throwException(TAG, ExceptionType::OPENGL, ("Unable to compile shader " + sid).c_str());
            return STATUS_KO;
        }
        ExceptionType exception;


        // Here we are good with our shaders
        // we can create a shader program and link it with the shaders
        //ShaderProgram* shaderProgram = new ShaderProgram();
        exception = shaderProgram->mLink(vertexHandle, fragmentHandle);
        if (exception != NO_EXCEPTION) {
            Log::error(TAG, "Unable to link shader program %s", sid.c_str());
            glDeleteShader(vertexHandle);
            glDeleteShader(fragmentHandle);
            assert(!"cannot link shader");
            throwException(TAG, exception, ("Unable to link shader " + sid).c_str());
            return STATUS_KO;
        }

        // we can mark shader to be deleted
        glDeleteShader(vertexHandle);
        glDeleteShader(fragmentHandle);

        // Bind the shader program locations
        Status status = shaderProgram->mBindLocations();
        if (status != STATUS_OK) {
            Log::error(TAG, "cannot bind location for shader program %s", sid.c_str());
            assert(!"cannot bind location for shader");
            return status;
        }

        Log::trace(TAG, "Shader %s loaded", sid.c_str());
        return STATUS_OK;
    }



    GLuint ShaderManager::mCompile(const std::string& source, GLenum type) const {
        GLuint handle = glCreateShader(type);
        const GLchar* src = source.c_str();
        glShaderSource(handle, 1, &src, NULL);
        glCompileShader(handle);

        //check errors
        GLint compile_ok = GL_FALSE;
        glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_ok);
        if (compile_ok == GL_FALSE) {
            printLog(handle);
            glDeleteShader(handle);
        }
        return handle;
    }



    void ShaderManager::update() {
        auto it = mShaderPrograms.begin();
        while (it != mShaderPrograms.end()) {
            if (it->second.unique()) {
                it->second->wipe();
                it = mShaderPrograms.erase(it);
            } else {
                ++it;
            }
        }
    }
}
