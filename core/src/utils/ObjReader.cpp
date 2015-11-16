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



#include "utils/ObjReader.hpp"
#include "utils/Log.hpp"

#include <sstream>
#include <cassert>

constexpr auto TAG = "ObjReader";

namespace dma {


    ObjReader::ObjReader(const std::string& path) :
                        mInputStream(path) {
        //check that file exists
        bool exists = mInputStream.is_open();
        if (!exists) {
            Log::error(TAG, "file " + path + " doesn't exist.");
            assert(!"error while loading obj");
        }
    }

    ObjReader::~ObjReader() {
        if(mInputStream.is_open()) {
            mInputStream.close();
        }
    }


    bool ObjReader::isOpen() const {
        return mInputStream.is_open();
    }

    Status ObjReader::mGotoLabel(const std::string& label) {
        std::string line;
        mInputStream.clear();
        mInputStream.seekg(0, std::ios::beg);
        while (getline(mInputStream, line)) {

            if (line.substr(0, label.length()) == label) {
                long seek = -line.size() - 1;
                mInputStream.seekg(seek, std::ios::cur);
                return STATUS_OK;
            }
        }
        return STATUS_KO;
    }


    Status ObjReader::gotoPositions() {
        return mGotoLabel("v ");
    }

    Status ObjReader::gotoNormals() {
        return mGotoLabel("vn ");
    }

    Status ObjReader::gotoFaces() {
        return mGotoLabel("f ");
    }

    Status ObjReader::gotoUV() {
        return mGotoLabel("vt ");
    }


    bool ObjReader::nextPosition(glm::vec3 &position) {
        std::string line;
        if(!getline(mInputStream, line)) return false;
        if(line.substr(0,2) != "v ") return false;
        std::istringstream s(line.substr(2));
        s >> position.x; s >> position.y; s >> position.z;
        return true;
    }


    bool ObjReader::nextNormal(glm::vec3 &normal) {
        std::string line;
        if(!getline(mInputStream, line)) return false;
        if(line.substr(0,3) != "vn ") return false;
        std::istringstream s(line.substr(3));
        s >> normal.x; s >> normal.y; s >> normal.z;
        return true;
    }

    bool ObjReader::nextUV(glm::vec2 &uv) {
        std::string line;
        if(!getline(mInputStream, line)) return false;
        if(line.substr(0,3) != "vt ") return false;
        std::istringstream s(line.substr(3));
        s >> uv.x; s >> uv.y;
        return true;
    }

    /**
     * f 2/3/2 2/2/1 1/1/3
     * f 2//2 2//1 1//3
     * f 2/2 2/1 1/3
     * f 2 2 1
     */

    bool ObjReader::nextFace(U16 face[3][3]) {
        std::string line;
        if(!getline(mInputStream, line)) return false;
        if(line.substr(0,2) != "f ") return false;
        std::istringstream s(line.substr(2));
        for(U32 i = 0; i < 3; ++i) {
            U16 index;
            s >> index;
            face[i][FACE_P] = (U16) (index - 1);
            if(s.get() == '/') {
                if(s.get() != '/') { // case uv, ie 1/2/2 or 1/2
                    s.unget();
                    s >> index;
                    face[i][FACE_UV] = (U16) (index - 1);
                } else { //there is no uv.
                    s.unget();
                    face[i][FACE_UV] = 0xFFFF; // means no uv since indices are one based.
                }
                if(s.get() == '/') { //there are normals
                    s >> index;
                    face[i][FACE_N] = (U16) (index - 1);
                } else {
                    face[i][FACE_N] = 0xFFFF; // no normal
                }
            } else {
                face[i][FACE_UV] = 0xFFFF; // means no uv since indices are one based.
                face[i][FACE_N] = 0xFFFF; // no normal
            }
        }
        return true;
    }

}
