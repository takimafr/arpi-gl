/*
Copyright © 2015 by eBusiness Information
All rights reserved. This source code or any portion thereof
may not be reproduced or used in any manner whatsoever
without the express written permission of eBusiness Information.
*/

#include "utils/Log.hpp"
#include <cstdarg>
#define LOG_COLOR

#ifdef LOG_COLOR
#define ANSI_COLOR_RED     "\x1B[31m"
#define ANSI_COLOR_GREEN   "\x1B[32m"
#define ANSI_COLOR_YELLOW  "\x1B[33m"
#define ANSI_COLOR_BLUE    "\x1B[34m"
#define ANSI_COLOR_MAGENTA "\x1B[35m"
#define ANSI_COLOR_CYAN    "\x1B[36m"
#define ANSI_COLOR_GRAY    "\x1B[37m"
#define ANSI_COLOR_RESET   "\x1B[0m"
#else
#define ANSI_COLOR_RED     ""
#define ANSI_COLOR_GREEN   ""
#define ANSI_COLOR_YELLOW  ""
#define ANSI_COLOR_BLUE    ""
#define ANSI_COLOR_MAGENTA ""
#define ANSI_COLOR_CYAN    ""
#define ANSI_COLOR_RESET   ""
#endif

/*
 * The linux implementation of the Log interface
 */

#define BUFFER_SIZE 256

namespace dma {


    void Log::info(const std::string& tag, const std::string& message, ...) {
        char buffer[BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, BUFFER_SIZE, message.c_str(), args);
        va_end(args);
        std::cout << ANSI_COLOR_CYAN << "[INFO] " << tag << ": " << buffer << ANSI_COLOR_RESET << std::endl;
    }

    void Log::warn(const std::string& tag, const std::string& message, ...) {
        char buffer[BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, BUFFER_SIZE, message.c_str(), args);
        va_end(args);
        std::cout << ANSI_COLOR_YELLOW << "[WARN] " << tag << ": " << buffer << ANSI_COLOR_RESET << std::endl;

    }
    void Log::error(const std::string& tag, const std::string& message, ...) {
        char buffer[BUFFER_SIZE];
        va_list args;
        va_start (args, message);
        vsnprintf(buffer, BUFFER_SIZE, message.c_str(), args);
        va_end (args);
        std::cerr << ANSI_COLOR_RED << "[ERROR] " << tag << ": " << buffer << ANSI_COLOR_RESET << std::endl;
    }

#ifdef DEBUG
    void Log::debug(const std::string& tag, const std::string& message, ...) {
        char buffer[BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, BUFFER_SIZE, message.c_str(), args);
        va_end(args);
        buffer[255] = '\0';
        std::cout << ANSI_COLOR_GREEN << "[DEBUG] " << tag << ": " << buffer << ANSI_COLOR_RESET << std::endl;
    }
#endif  //debug

#ifdef TRACE

    void Log::trace(const std::string& tag, const std::string& message, ...) {
        char buffer[BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsnprintf(buffer, BUFFER_SIZE, message.c_str(), args);
        va_end(args);
        std::cout << ANSI_COLOR_GRAY << "[TRACE] " << tag << ": " << buffer << ANSI_COLOR_RESET << std::endl;
    }

#endif  //trace
}
