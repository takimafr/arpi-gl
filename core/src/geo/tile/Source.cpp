#include "geo/tile/Source.hpp"

namespace dma {

    Source::Type Source::type(const std::string& type) {
        if (type == "raster") {
            return RASTER;
        } else if (type == "vector") {
            return VECTOR;
        } else {
            return UNKNOWN;
        }
    }

}


