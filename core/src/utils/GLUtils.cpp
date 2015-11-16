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




#include "utils/GLUtils.hpp"
#include "utils/GLES2Logger.hpp"
#include "utils/Log.hpp"
#include "utils/ExceptionHandler.hpp"

#include <GLES2/gl2.h>

#include <cstring>


using namespace dma;
constexpr char GLUtils::TAG[];

bool GLUtils::hasGlError(const std::string& tag, const std::string& op) {
    bool hasError = false;
    for(GLint error = glGetError(); error; error = glGetError()) {
        hasError = true;
        std::string errorStr;
        char errBuff[8];
        sprintf(errBuff, "0x%x", error);
        errorStr = errBuff;
        errorStr += " : " + getGlMessage(error);
        Log::error(tag, "after %s glError(%s)\n", op.c_str(), errorStr.c_str());
        throwException(tag, ExceptionType::OPENGL, NULL);
    }
    return hasError;
}

std::string GLUtils::getGlMessage(int errorCode) {
    switch(errorCode) {
                case 0 :
                    return "GL_NO_ERROR";
                case 0x0500:
                    return "GL_INVALID_ENUM";
                case 0x0501:
                    return "GL_INVALID_VALUE";
                case 0x0502:
                    return "GL_INVALID_OPERATION";
                case 0x0505:
                    return "GL_OUT_OF_MEMORY";

                default :
                    break;
            }
    char errBuff[8];
    sprintf(errBuff, "0x%x", errorCode);

    return "code(" + std::string(errBuff) +")";

}


void GLUtils::clearGlErrors() {
    for(GLint error = glGetError(); error; error = glGetError()) {
        Log::warn(TAG, "discarding gl error %x", error);
    }
}


bool GLUtils::isExtSupported(const std::string& extension) {

    const char* c_extension = extension.c_str();

    GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;

    where = (GLubyte *) strchr(c_extension, ' ');
    if (where || *c_extension == '\0')
        return 0;

    extensions = (GLubyte *)glGetString(GL_EXTENSIONS);

    start = extensions;
    for (;;)
    {
        where = (GLubyte *) strstr((const char *) start, c_extension);

        if (!where)
            break;

        terminator = where + strlen(c_extension);

        if (where == start || *(where - 1) == ' ')
        {
            if (*terminator == ' ' || *terminator == '\0')
                return 1;
        }
        start = terminator;
    }
    return 0;
}

bool GLUtils::hasGlContext() {
    //check openGL version & context
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    if (!renderer) {
        return false;
    }

    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    return (bool) version != 0;

}

void GLUtils::printGlContext() {
    //check openGL version & context
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    const GLubyte* vendor = glGetString(GL_VENDOR); // vendor as a string
    const GLubyte* extensions = glGetString(GL_EXTENSIONS); // extensions as a string

    bool errors = false;
    if (!renderer) {
        Log::error(TAG, "no renderer found!!");
        errors = true;
    }

    if (!version) {
        Log::trace(TAG, "no valid openGL context found!!");
        errors = true;
    }

    // no tolerance for errors in DEBUG
    if (errors) {
        assert(false);
    } else {
        Log::trace(TAG, "Renderer: %s\n", renderer);
        Log::trace(TAG, "Version: %s\n", version);
        Log::trace(TAG, "Vendor: %s\n", vendor);
        int length = strlen((char*)extensions);
        const int buffSize = 127;
        char buff [buffSize+1];
        Log::trace(TAG, "Extensions: ");

        for(int i=0; i<length; i+=buffSize) {
            if(i>length) {
                i = length;
            }
            strncpy(buff, &((char*)extensions)[i], buffSize);
            buff[buffSize] = '\0';
            Log::trace(TAG, "%s", buff);

        }
    }
}
