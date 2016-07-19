#ifndef ARPIGL_TILEPROVIDER_HPP
#define ARPIGL_TILEPROVIDER_HPP

#include <memory>
#include <common/Types.hpp>
#include <vector>

namespace dma {
    class TileProvider {
    public:
        TileProvider(const std::string& uri);
        virtual ~TileProvider();

        virtual std::vector<BYTE> fetch(int x, int y, int z) = 0;

    protected:
        std::string formatUri(int x, int y, int z);

        std::string mUri;
    };
}

#endif //ARPIGL_TILEPROVIDER_HPP
