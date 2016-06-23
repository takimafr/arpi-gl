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


    /* ================= MEMBERS ========================*/

    ShaderManager::ShaderManager(const std::string& localDir) :
            GpuResourceManagerHandler(localDir)
    {}

    ShaderManager::~ShaderManager() {
    }


    bool ShaderManager::hasResource(const std::string& sid) const {
        const std::string& path = mLocalDir + sid;
        bool res = Utils::fileExists(path + ".v.glsl") || Utils::fileExists(path + ".v.GLSL");
        res &=  (Utils::fileExists(path + ".f.glsl") || Utils::fileExists(path + ".f.glsl"));
        return res;
    }


    void ShaderManager::load(std::shared_ptr<ShaderProgram> shaderProgram, const std::string &sid) {
        Log::trace(TAG, "Loading shader %s ...", sid.c_str());

        //try to load from the cache
        if (shaderProgram->hasCache()) {
            load(shaderProgram, sid, shaderProgram->mVertexSource, shaderProgram->mFragmentSource);
        }

        //otherwise load from the file

        std::string vertexSource;
        std::string fragmentSource;
        Utils::bufferize(mLocalDir + "/" + sid + ".v.glsl", vertexSource);
        Utils::bufferize(mLocalDir + "/" + sid + ".f.glsl", fragmentSource);


        //update the cache
        shaderProgram->mVertexSource = vertexSource;
        shaderProgram->mFragmentSource = fragmentSource;

        return load(shaderProgram, sid, vertexSource, fragmentSource);
    }


    void ShaderManager::load(std::shared_ptr<ShaderProgram> shaderProgram,
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
        GLuint vertexHandle = compile(vertexSource, GL_VERTEX_SHADER);
        GLuint fragmentHandle = compile(fragmentSource, GL_FRAGMENT_SHADER);

        if (vertexHandle == 0 || fragmentHandle == 0) {
            std::string error = "Unable to compile shader " + sid;
            Log::error(TAG, error.c_str());
            throw std::runtime_error(error);
        }
        ExceptionType exception;


        // Here we are good with our shaders
        // we can create a shader program and link it with the shaders
        //ShaderProgram* shaderProgram = new ShaderProgram();
        exception = shaderProgram->mLink(vertexHandle, fragmentHandle);
        if (exception != NO_EXCEPTION) {
            std::string error = "Unable to link shader program " + sid;
            Log::error(TAG, error.c_str());
            glDeleteShader(vertexHandle);
            glDeleteShader(fragmentHandle);
            throw std::runtime_error(error);
        }

        // we can mark shader to be deleted
        glDeleteShader(vertexHandle);
        glDeleteShader(fragmentHandle);

        // Bind the shader program locations
        Status status = shaderProgram->mBindLocations();
        if (status != STATUS_OK) {
            std::string error = "cannot bind location for shader program " + sid;
            Log::error(TAG, error.c_str());
            throw std::runtime_error(error);
        }

        Log::trace(TAG, "Shader %s loaded", sid.c_str());
    }



    GLuint ShaderManager::compile(const std::string &source, GLenum type) const {
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
}
