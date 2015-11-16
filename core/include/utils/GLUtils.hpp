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


/*
 * GLUtils.hpp
 *
 *  Created on: 15 juin 2015
 *      Author: excilys
 */

#ifndef DMA_GLUTILS_HPP_
#define DMA_GLUTILS_HPP_

#include <cstdio>
#include <string>

namespace dma {
    class GLUtils {
        static constexpr char TAG[] = "GLUtils";

    public:

        /**
         * return true if errors
         */
        static bool hasGlError(const std::string& tag, const std::string& op);

        static std::string getGlMessage(int errorCode);

        static void clearGlErrors();

        /**
         * @return true if the extension identified by parameter is supported by the current platform.
         */
        static bool isExtSupported(const std::string& extension);

        static void printGlContext();

        static bool hasGlContext();

    };  // GLES2Logger

} //dma





#endif /* DMA_GLUTILS_HPP_ */
