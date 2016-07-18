#include "geo/tile/Layer.hpp"

namespace dma {

    Layer::Type Layer::type(const std::string& type) {
        if (type == "extrude") {
            return EXTRUDE;
        } else if (type == "fill") {
            return FILL;
        } else if (type == "line") {
            return LINE;
        } else if (type == "circle") {
            return CIRCLE;
        } else if (type == "symbol") {
            return SYMBOL;
        } else if (type == "raster") {
            return RASTER;
        } else if (type == "background") {
            return BACKGROUND;
        } else {
            return UNKNOWN;
        }
    }

}
