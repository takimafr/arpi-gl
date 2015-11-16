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


#ifndef _DMA_EXCEPTION_HANDLER_HPP_
#define _DMA_EXCEPTION_HANDLER_HPP_

#include <string>
#include <iostream>
#include <cassert>
#include "common/Types.hpp"
#include "utils/Log.hpp"
#include "utils/ExceptionType.hpp"
namespace dma {

#define throwException(tag, type, message) ExceptionHandler::__DMA__throwException(tag, type, __LINE__, __FILE__, __FUNCTION__,  message)
#define throwError(tag, type, message) ExceptionHandler::__DMA__throwError(tag, type, __LINE__, __FILE__, __FUNCTION__,  message)
    class ExceptionHandler {
    public:
        static inline Status __DMA__throwError(const std::string& tag, dma::Status type, int line, const char* file, const char* func, const std::string& what) {
            return __DMA__throwError(tag, type, line, file, func, what);
        }

        static inline Status __DMA__throwException(const std::string& tag, dma::Status type, int line, const char* file, const char* func, const std::string& what) {
            return __DMA__throwException(tag, type, line, file, func, what);
        }

        static inline Status __DMA__throwError(const std::string& tag, dma::ExceptionType type, int line, const char* file, const char* func, const std::string& what) {
            Log::error(tag, what);
            return (Status)type;
        }

        static inline Status __DMA__throwException(const std::string& tag, dma::ExceptionType type, int line, const char* file, const char* func, const std::string& what) {
            return __DMA__throwError(tag, type, line, file, func, what);
        }

    };
}

#endif /* _DMA_EXCEPTION_HANDLER_HPP_ */
