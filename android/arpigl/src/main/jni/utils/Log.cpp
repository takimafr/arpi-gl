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
#include "utils/Log.hpp"
#include <cstdarg>
#include <android/log.h>
/*
 * The android implementation of the Log interface
 */
namespace dma {

    void Log::error(const std::string& tag, const std::string& message, ...) {
        char buffer[256];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, 256, message.c_str(), args);
        va_end(args);
        __android_log_print(ANDROID_LOG_ERROR, tag.c_str(), "%s", buffer);
        fflush(stdout);
        fflush(stderr);
    }

    void Log::warn(const std::string& tag, const std::string& message, ...) {
        char buffer[256];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, 256, message.c_str(), args);
        va_end(args);
        __android_log_print(ANDROID_LOG_WARN, tag.c_str(), "%s", buffer);
        fflush(stdout);
        fflush(stderr);
    }

    void Log::info(const std::string& tag, const std::string& message, ...) {
        char buffer[256];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, 256, message.c_str(), args);
        va_end(args);
        __android_log_print(ANDROID_LOG_INFO, tag.c_str(), "%s", buffer);
        fflush(stdout);
        fflush(stderr);
    }

#ifdef DEBUG
    void Log::debug(const std::string& tag, const std::string& message, ...) {
        char buffer[256];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, 256, message.c_str(), args);
        va_end(args);
        __android_log_print(ANDROID_LOG_DEBUG, tag.c_str(), "%s", buffer);
        fflush(stdout);
        fflush(stderr);
    }

#ifdef TRACE
    void Log::trace(const std::string& tag, const std::string& message, ...) {
        char buffer[256];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, 256, message.c_str(), args);
        va_end(args);
        __android_log_print(ANDROID_LOG_VERBOSE, tag.c_str(), "%s", buffer);
        fflush(stdout);
        fflush(stderr);
    }
#endif  // TRACE
#endif  // DEBUG
}
