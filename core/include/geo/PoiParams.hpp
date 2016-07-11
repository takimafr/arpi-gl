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


#ifndef _DMA_POIPARAMS_HPP_
#define _DMA_POIPARAMS_HPP_

#include "utils/Color.hpp"

#include <string>

namespace dma {
    class PoiParams { //TODO remove ?

    public:
        PoiParams(const std::string &sid, const std::string &shape,
                  const std::string &icon, const Color &color,
                  double lat, double lng, double alt) :
                sid(sid), shape(shape),
                icon(icon), color(color),
                lat(lat), lng(lng), alt(alt)
        {}

        PoiParams(const std::string &sid, const std::string &shape,
                  const std::string &icon, const Color &color) :
                sid(sid), shape(shape),
                icon(icon), color(color),
                lat(0.0), lng(0.0), alt(0.0)
        {}

        std::string sid;
        std::string shape;
        std::string icon;
        Color color;
        double lat;
        double lng;
        double alt;
    };
}

#endif //_DMA_POIPARAMS_HPP_
