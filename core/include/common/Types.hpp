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


#ifndef _DMA_TYPES_HPP_
#define _DMA_TYPES_HPP_

#include <stdint.h>

namespace dma {

    typedef uint8_t U8;
    typedef uint16_t U16;
    typedef int32_t I32;
    typedef uint32_t U32;
    typedef int64_t I64;
    typedef uint64_t U64;

    typedef float F32;
    typedef double F64;

    typedef unsigned char BYTE;

    //status
    enum Status {
        STATUS_OK = true, STATUS_KO = -1, STATUS_EXIT = -2
    };

}

#endif /* _DMA_TYPES_HPP_ */
