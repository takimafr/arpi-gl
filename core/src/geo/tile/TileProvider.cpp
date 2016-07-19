#include <utils/Utils.hpp>
#include "geo/tile/TileProvider.hpp"

namespace dma {

    TileProvider::TileProvider(const std::string& uri) :
            mUri(uri)
    {}

    TileProvider::~TileProvider() {

    }

    std::string TileProvider::formatUri(int x, int y, int z) {
        std::string uri = mUri;
        Utils::replace(uri, "{x}", std::to_string(x));
        Utils::replace(uri, "{y}", std::to_string(y));
        Utils::replace(uri, "{z}", std::to_string(z));
        return std::move(uri);
    }

}



