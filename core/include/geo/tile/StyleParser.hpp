#ifndef ARPIGL_STYLEPARSER_HPP
#define ARPIGL_STYLEPARSER_HPP

#include <rapidjson/document.h>
#include "geo/tile/Source.hpp"
#include "geo/tile/Layer.hpp"
#include "geo/tile/Style.hpp"

namespace dma {

    class StyleParser {

    public:
        Style parse(const std::string& json);

    private:
        void parseSources(const rapidjson::Value& value);

        void parseLayers(const rapidjson::Value& value);

        Layer parseLayer(const std::string& id, const rapidjson::Value& value);

        void parseTileJson(const rapidjson::Value& value, Source& source);

        std::map<std::string, Source> mSources;
        std::vector<Layer> mLayers;
    };

}



#endif //ARPIGL_STYLEPARSER_HPP
