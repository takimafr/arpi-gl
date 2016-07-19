#ifndef ARPIGL_FILETILEPROVIDER_HPP
#define ARPIGL_FILETILEPROVIDER_HPP

#include "geo/tile/TileProvider.hpp"

namespace dma {
    class FileTileProvider : public TileProvider {

    public:
        FileTileProvider(const std::string &uri);

        virtual std::vector<BYTE> fetch(int x, int y, int z) override;

    };
}


#endif //ARPIGL_FILETILEPROVIDER_HPP
