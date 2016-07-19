#ifndef ARPIGL_SOURCE_HPP
#define ARPIGL_SOURCE_HPP

#include <string>
#include <vector>
#include <common/Types.hpp>
#include "TileProvider.hpp"

namespace dma {
    class Source {

        friend class StyleParser;

    public:
        enum Type { VECTOR, RASTER, UNKNOWN };

        static Type type(const std::string& type);

        ~Source();

        std::vector<BYTE> fetch(int x, int y, int z) const;

    private:
        std::string mId;
        Type mType;
        std::string mUrl;
        std::vector<std::string> mTiles;
        U8 mMinZoom;
        U8 mMaxZoom;
        std::shared_ptr<TileProvider> mTileProvider;
    };
}


#endif //ARPIGL_SOURCE_HPP
