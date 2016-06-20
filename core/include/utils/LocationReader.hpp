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


#ifndef _DMA_LOCATION_READER_HPP_
#define _DMA_LOCATION_READER_HPP_

#include "common/Types.hpp"
#include "rapidjson.h"
#include "document.h"
#include "glm/glm.hpp"

#include <string>
#include <fstream>


namespace dma {

    class LocationReader {
    private:
        std::string mPath;
        rapidjson::Document mDocument;

    public:

        /**
        * Creates a new LocationReader ready to parse the file
        * supplied as parameter.
        */
        LocationReader(const std::string& path);
        LocationReader(const LocationReader&) = delete;
        void operator=(const LocationReader&) = delete;
        virtual ~LocationReader();

        /**
        * 1. Open the file
        * 2. Stringify it
        * 3. Create the DOM ready to be queried
        * Must be called before any other method
        */
        Status parse();
    };
}

#endif