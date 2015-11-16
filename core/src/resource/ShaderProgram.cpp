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



#include "resource/ShaderProgram.hpp"
#include "utils/ExceptionHandler.hpp"


namespace dma {

    /* ================= STATIC VARIABLES ========================*/
    constexpr char ShaderProgram::TAG[];

    const std::string ShaderProgram::attributeNames[] = { "a_position", "a_normal", "a_uv" };
    const std::string ShaderProgram::uniformNames[] = {
            "u_MV",
            "u_MVP",
            "u_N",
            "u_diffuse_map",
            "u_diffuse_map_enabled",
            "u_cube_map",
            "u_diffuse_color",
            "u_light0",
            "u_light0.position",
            "u_light0.La",
            "u_light0.Ld",
            "u_light0.Ls"
    };


    /* ================= PUBLIC ========================*/

    //------------------------------------------------------------------------------
    ShaderProgram::ShaderProgram() :
            mHandle(0), mAttributeFlags(0L), mUniformFlags(0L){
    }


    //------------------------------------------------------------------------------
    ShaderProgram::~ShaderProgram() {
    }

    /* ================= PRIVATE ========================*/

    //------------------------------------------------------------------------------
    ExceptionType ShaderProgram::mLink(GLuint vertexHandle, GLuint fragmentHandle) {
        GLint link_ok;
        mHandle = glCreateProgram();
        glAttachShader(mHandle, vertexHandle);
        glAttachShader(mHandle, fragmentHandle);
        glLinkProgram (mHandle);
        glGetProgramiv(mHandle, GL_LINK_STATUS, &link_ok);
        if (!link_ok) {
            glDeleteProgram(mHandle);
            Log::error(TAG, "Linking error");
            mHandle = 0;
            return ExceptionType::OPENGL;
        }

        return ExceptionType::NO_EXCEPTION;
    }


    //------------------------------------------------------------------------------
    Status ShaderProgram::mBindLocations() {
        std::string name;
        GLint location;
        GLenum error;
        for (U32 i = 0; i < AS_size; ++i) {
            name = ShaderProgram::attributeNames[i];
            location = glGetAttribLocation(mHandle, name.c_str());
            error = glGetError();
            if (error != GL_NO_ERROR) {
                Log::error(TAG, "Error while binding location for attribute %s (error : %x)", name.c_str(), error);
                assert(!"Error while binding location for attribute");
                return throwException(TAG, ExceptionType::OPENGL,
                        "Error while binding location for attribute " + name + " - error : " + GLUtils::getGlMessage(error));
            }
            mAttributeLocations[i] = location;
            if (location != -1) {
                mAttributeFlags |= (1L << i);
            }
        }

        for (U32 i = 0; i < US_size; ++i) {
            name = ShaderProgram::uniformNames[i];
            location = glGetUniformLocation(mHandle, name.c_str());
            error = glGetError();
            if (error != GL_NO_ERROR) {
                Log::error(TAG, "Error while binding location for uniform %s (error : %x)", name.c_str(), error);
                assert(!"Error while binding location for uniform");
                throwException(TAG, ExceptionType::OPENGL, "Error while binding location for uniform " + name + " - error : " + GLUtils::getGlMessage(error));
                return STATUS_KO;
            }
            mUniformLocations[i] = location;
            if (location != -1) {
                mUniformFlags |= (1L << i);
            }
        }
        return STATUS_OK;
    }


    //------------------------------------------------------------------------------
    void ShaderProgram::wipe() {
        if (glIsProgram(mHandle)) {
            Log::debug(TAG, "deleting shader program %d", mHandle);
            glDeleteProgram(mHandle);
        }
        mHandle = 0;
    }

}
