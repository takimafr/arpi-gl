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


#ifndef _DMA_VERTEXINDICES_HPP_
#define _DMA_VERTEXINDICES_HPP_

#include "common/Types.hpp"

namespace dma {

    struct VertexIndices {
        U16 p, uv, fn, sn;

        VertexIndices() : p(0xFFFF), uv(0xFFFF), fn(0xFFFF), sn(0xFFFF)
        {}

        VertexIndices(U16 p, U16 uv, U16 fn) : p(p), uv(uv), fn(fn), sn(0xFFFF)
        {}

        bool operator<(const VertexIndices& other) const {
            if(p < other.p) {
                return true;
            } else if (p == other.p) {
                if(uv < other.uv) {
                    return true;
                } else if(uv == other.uv) {
                    return fn<other.fn;
                }
            }
            return false;
        }
    };
}

#endif //_DMA_VERTEXINDICES_HPP_
