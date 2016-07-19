#include <utils/ExceptionUtils.hpp>
#include <vector>
#include <algorithm>
#include <utils/Utils.hpp>
#include "geo/tile/FileTileProvider.hpp"

constexpr auto TAG = "FileTileProvider";

namespace dma {

    FileTileProvider::FileTileProvider(const std::string &uri) :
        TileProvider(uri)
    {
        // Assert uri is well formatted
        std::string prefix = "file://";
        if (uri.compare(0, prefix.size(), prefix)) {
            ExceptionUtils::runtime(TAG, "uri must start with 'file://'");
        }

        // Remove prefix for convenience
        mUri.erase(0, prefix.size());
    }

    std::vector<BYTE> FileTileProvider::fetch(int x, int y, int z) {
        try {
            return Utils::bufferize(formatUri(x, y, z));
        } catch (const std::exception& e) {
            Log::warn(TAG, "Unable to fetch tile " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z));
            return std::vector<BYTE>();
        }
    }


}



