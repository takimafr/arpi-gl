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


#ifndef DMA_UTILS_HPP
#define DMA_UTILS_HPP


// std
#include <string>
#include <vector>
#include <cassert>
#include <sstream>

// gles
#include "utils/GLES2Logger.hpp"


#include "common/Types.hpp"
#include "Log.hpp"

#include "glm/glm.hpp"



namespace dma {
    class Utils {

    public:

        static std::vector<std::string> &split(const std::string &s,
                                               char delim,
                                               std::vector<std::string> &elems);


        static long getFileSize(const std::string& path);

        /*
         * Fills the buffer with the content of the resource file.
         * @param const std::string
         *              path of the text file to open
         * @param std::string&
         *              string that will be filled by file's content.
         */
        static Status bufferize(const std::string& path, std::string& buffer);


        static std::vector<BYTE> bufferize(const std::string& path);

        static bool dirExists(const char *path);

        /**
         * @return true if the given file exists.
         */
        static bool fileExists(const std::string&);

        static std::string getFileExt(const std::string& s);

        /**
         * Appends ".ext" at the end of filename if not already present
         * ex: 1. addFile("foo", "png") will return "foo.png"
         *     2. addFile("foo.png", "png") will return "foo.png"
         * /!\ addFile("foo", ".png") will return "foo..png"
         */
        static std::string& addFileExt(std::string& filename, const std::string& ext);

        static std::string& addTrailingSlash(std::string& filename);

        static std::string replace(std::string& str, const std::string& target, const std::string& replacement);

    };


    inline const std::string to_string(int value) {
        std::ostringstream os ;
        os << value ;
        return os.str() ;
    }
}

#endif //DMA_UTILS_HPP
