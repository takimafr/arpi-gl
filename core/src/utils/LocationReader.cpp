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



#include "utils/LocationReader.hpp"
#include "utils/Log.hpp"
#include "utils/ExceptionHandler.hpp"
#include "utils/Utils.hpp"

constexpr auto TAG = "LocationReader";

namespace dma {

    LocationReader::LocationReader(const std::string& path) :
            mPath(path)
    {}


    LocationReader::~LocationReader() {

    }

    Status LocationReader::parse() {
        // 1. Stringify the file
        std::string json;
        Status status = Utils::bufferize(mPath, json);
        if (status != STATUS_OK) {
            Log::error(TAG, "Unable to bufferize Locations %s", mPath.c_str());
            assert(!"Unable to bufferize Locations");
            return status;
        }

        // 2. Create the DOM
        mDocument.Parse(json.c_str());
        if (mDocument.HasParseError()) {
            Log::error(TAG, "Unable to parse Locations %s", mPath.c_str());
            assert(!"Unable to parse Locations");
            return throwException(TAG, ExceptionType::PARSE_ERROR, "Unable to parse Locations " + mPath);
        }
        for (rapidjson::Value::ConstMemberIterator itr = mDocument.MemberBegin(); itr != mDocument.MemberEnd(); ++itr) {
            std::cout << itr->value["lat"].GetDouble() << std::endl;
        }

        return STATUS_OK;
    }
}

