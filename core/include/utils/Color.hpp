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


#ifndef _DMA_COLOR_HPP_
#define _DMA_COLOR_HPP_

namespace dma {

    struct Color {
        Color() {}
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
        Color(float r, float g, float b) : Color(r, g, b, 1.0f) {}
        Color(int r, int g, int b) : Color(r/255.f, g/255.f, b/255.f) {}
        float r;
        float g;
        float b;
        float a;
    };
}

#endif //_DMA_COLOR_HPP_
