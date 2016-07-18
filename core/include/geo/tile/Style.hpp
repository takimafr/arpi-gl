#ifndef ARPIGL_STYLE_HPP
#define ARPIGL_STYLE_HPP

#include <vector>
#include "geo/tile/Layer.hpp"

namespace dma {
    class Style {

        friend class StyleParser;

    public:
        inline std::vector<Layer>& getLayers() {
            return mLayers;
        }

        inline const std::map<std::string, Source>& getSources() {
            return mSources;
        };

    private:
        std::map<std::string, Source> mSources;
        std::vector<Layer> mLayers;
    };
}


#endif //ARPIGL_STYLE_HPP
