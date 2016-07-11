#include <utils/Log.hpp>
#include "geo/VectorTile.hpp"

#define TAG "VectorTile"

namespace dma {

    VectorTile::VectorTile(vector_tile::Tile tile) {
        for (auto layer : tile.layers()) {
            if (layer.name() == "building") {

            }
        }
    }

}

