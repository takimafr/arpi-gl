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


#ifndef _DMA_LOG_HPP_
#define _DMA_LOG_HPP_

#include <string>
#include <iostream>

namespace dma {

    class Log {
    public:
        static void error(const std::string& tag, const std::string& message, ...);
        static void warn(const std::string& tag, const std::string& message, ...);
        static void info(const std::string& tag, const std::string& message, ...);
#ifdef DEBUG
        static void debug(const std::string& tag, const std::string& message, ...);
#ifdef TRACE
        static void trace(const std::string& tag, const std::string& message, ...);
#endif
#endif
#ifndef DEBUG
        static inline void debug(const std::string& tag, const std::string& message, ...) {
        }
#endif
#ifndef TRACE
        static inline void trace(const std::string& tag, const std::string& message, ...) {
        }
#endif
    };
}

#endif /* _DMA_LOG_HPP_ */
