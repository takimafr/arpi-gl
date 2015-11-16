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


#ifndef _DMA_OBJREADER_HPP_
#define _DMA_OBJREADER_HPP_

#include "common/Types.hpp"
#include "glm/glm.hpp"

#include <string>
#include <fstream>

namespace dma {
        class ObjReader {
        public:
            static const int FACE_P = 0;
            static const int FACE_UV = 1;
            static const int FACE_N = 2;

            ObjReader(const std::string& path);
            ObjReader(const ObjReader&) = delete;
            void operator=(const ObjReader&) = delete;
            virtual ~ObjReader();

            bool isOpen() const;

            Status gotoPositions();
            Status gotoNormals();
            Status gotoFaces();
            Status gotoUV();

            bool nextPosition(glm::vec3& position);
            bool nextNormal(glm::vec3& normal);
            bool nextUV(glm::vec2& );
            bool nextFace(U16 face[3][3]);

        private:
            std::ifstream mInputStream;
            Status mGotoLabel(const std::string&);

        };
    }

#endif //_DMA_OBJREADER_HPP_
