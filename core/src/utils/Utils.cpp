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



#include <fstream>
#include <iterator>
#include <sys/stat.h>
#include <stdexcept>
#include "utils/Utils.hpp"
#include "utils/ExceptionHandler.hpp"


constexpr auto TAG = "Utils";

namespace dma {


    std::vector<std::string> &Utils::split(const std::string &s, char delim,
                                           std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }



    bool Utils::dirExists(const char *path) {
        struct stat info;
        if (stat(path, &info) != 0)
            return false;
        else return (info.st_mode & S_IFDIR) != 0;
    }


    bool Utils::fileExists(const std::string& path) {
        std::ifstream is;
        is.open(path.c_str());
        bool res = is.is_open();
        is.close();
        return res;

    }



    long Utils::getFileSize(const std::string& path) {
        long length;
        std::ifstream is;
        is.open(path.c_str(), std::ios::ate);
        if (!is.is_open()) {
            Log::error(TAG, "Cannot open file %s", path.c_str());
            return -1;
        }
        length = is.tellg();
        return length;
    }



    Status Utils::bufferize(const std::string& path, std::string& buffer) {
        I64 length;
        std::ifstream is;
        is.open(path.c_str(), std::ios::ate);
        if (!is.is_open()) {
            std::string error = "Cannot open file " + path;
            Log::error(TAG, error);
            throw std::runtime_error(error);
        }
        length = is.tellg();
        is.seekg(0);

        char* bufferTmp = new char[length + 1];

        // read data as a block:
        is.read(bufferTmp, length);
        is.close();
        bufferTmp[length] = '\0';
        buffer.assign(bufferTmp);
        delete[] bufferTmp;
        return STATUS_OK;
    }



    Status Utils::bufferize(const std::string& path, std::vector<BYTE>& buffer) {
        I64 length;
        std::ifstream is(path.c_str(), std::ios::binary | std::ios::ate);
        if (!is.is_open()) {
            Log::error(TAG, "Cannot open file %s", path.c_str());
            return throwException(TAG, ExceptionType::IO, "Cannot open file " + path);
        }
        // Stop eating new lines in binary mode
        is.unsetf(std::ios::skipws);

        length = is.tellg();
        is.seekg(0);
        buffer.reserve((unsigned long) length);
        // read the data:
        buffer.assign(std::istream_iterator<BYTE>(is),
                      std::istream_iterator<BYTE>());
        is.close();
        return STATUS_OK;
    }



    std::string Utils::getFileExt(const std::string &s) {
        size_t i = s.rfind('.', s.length());
        if (i != std::string::npos) {
            return(s.substr(i+1, s.length() - i));
        }

        return("");
    }



    std::string& Utils::addFileExt(std::string &filename, const std::string &ext) {
        std::string fileExt = Utils::getFileExt(filename);
        if (fileExt == ext) {
            return filename;
        } else {
            return filename.append(".").append(ext);
        }
    }



    std::string& Utils::addTrailingSlash(std::string& dir) {
        if (!dir.empty() && dir.at(dir.length() - 1) != '/') {
            dir += '/';
        }
        return dir;
    }
}
