#include <vector>
#include <algorithm>

#include <utils/ExceptionUtils.hpp>
#include <geo/tile/FileTileProvider.hpp>
#include <utils/Utils.hpp>
#include "geo/tile/StyleParser.hpp"

constexpr auto TAG = "StyleParser";
constexpr auto VERSION = 8;

namespace dma {

    StyleParser::StyleParser()
    {}

    Style dma::StyleParser::parse(const std::string& file) {
        Style style;

        std::string json;
        Utils::bufferize(file, json);
        mFile = file;

        rapidjson::Document document;

        // 1. Parse document
        document.Parse(json.c_str());
        if (document.HasParseError()) {
            ExceptionUtils::runtime(TAG, "Unable to parse style");
        }

        // 2. Check version
        if (!document.HasMember("version")) {
            ExceptionUtils::runtime(TAG, "style must have a version set to " + VERSION);
        }

        int version = document["version"].GetInt();
        if (version != VERSION) {
            ExceptionUtils::runtime(TAG, "style version must be " + VERSION);
        }

        // 3. Parse sources
        if (document.HasMember("sources")) {
            parseSources(document["sources"]);
        }

        // 4. Parse layers
        if (document.HasMember("layers")) {
            parseLayers(document["layers"]);
        }


        // 5. Parse sprites
        //TODO

        //6. Parse glyphs
        //TODO

        style.mSources = std::move(mSources);
        style.mLayers = std::move(mLayers);
        return style;
    }


    void StyleParser::parseSources(const rapidjson::Value &value) {
        if (!value.IsObject()) {
            ExceptionUtils::runtime(TAG, "sources must be an object");
        }

        // For each sources
        for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it) {
            Source source;

            // 1. Parse id
            source.mId = it->name.GetString();

            const rapidjson::Value &sourceVal = it->value;

            // 2. Parse type
            if (!sourceVal.HasMember("type")) {
                ExceptionUtils::runtime(TAG, "source " + source.mId + " must have a type");
            }
            const rapidjson::Value &typeVal = sourceVal["type"];
            if (!typeVal.IsString()) {
                ExceptionUtils::runtime(TAG, "source " + source.mId + " type must be a string");
            }
            source.mType = Source::type(typeVal.GetString());
            if (source.mType == Source::Type::UNKNOWN) {
                ExceptionUtils::runtime(TAG, "source " + source.mId + ": unkonwn type");
            }

            // 3. Parse url
            if (sourceVal.HasMember("url")) {
                const rapidjson::Value &urlVal = sourceVal["url"];
                if (!urlVal.IsString()) {
                    ExceptionUtils::runtime(TAG, "source " + source.mId + " url must be a string");
                }
                source.mUrl = {urlVal.GetString(), urlVal.GetStringLength()};
            }

            // 4. Parse tileJSON
            parseTileJson(sourceVal, source);

            mSources.emplace(source.mId, std::move(source));
        }
    }

    void StyleParser::parseLayers(const rapidjson::Value &value) {
        std::vector<std::string> ids;

        if (!value.IsArray()) {
            ExceptionUtils::runtime(TAG, "layers must be an array");
        }

        for (auto it = value.Begin(); it != value.End(); ++it) {
            const rapidjson::Value &layerVal = *it;

            if (!layerVal.IsObject()) {
                ExceptionUtils::runtime(TAG, "layer must be an object");
            }

            if (!layerVal.HasMember("id")) {
                ExceptionUtils::runtime(TAG, "layer must have an id");
            }
            const rapidjson::Value &idVal = layerVal["id"];
            if (!idVal.IsString()) {
                ExceptionUtils::runtime(TAG, "layer id must be a string");
            }

            // Asserts the layer id is unique
            const std::string id = {idVal.GetString(), idVal.GetStringLength()};
            if (std::find(ids.begin(), ids.end(), id) != ids.end()) {
                ExceptionUtils::runtime(TAG, "duplicated layer id " + id);
            }

            mLayers.push_back(parseLayer(id, layerVal));
        }

    }

    Layer StyleParser::parseLayer(const std::string &id, const rapidjson::Value &value) {
        Layer layer;
        layer.mId = id;

        // 1. Parse type
        if (!value.HasMember("type")) {
            ExceptionUtils::runtime(TAG, "layer " + layer.mId + " is missing a type");
        }
        const rapidjson::Value &typeVal = value["type"];
        if (!typeVal.IsString()) {
            ExceptionUtils::runtime(TAG, "layer " + layer.mId + ": type must be a string");
        }
        layer.mType = Layer::type(typeVal.GetString());
        if (layer.mType == Layer::Type::UNKNOWN) {
            ExceptionUtils::runtime(TAG, "layer " + layer.mId + ": type '" + typeVal.GetString() + "' is unknown");
        }

        // 2. Parse source
        if (value.HasMember("source")) {
            const rapidjson::Value &sourceVal = value["source"];
            if (!sourceVal.IsString()) {
                ExceptionUtils::runtime(TAG, "layer " + layer.mId + ": source must be a string");
            }
            // Ensure the source exists
            if (mSources.find(sourceVal.GetString()) == mSources.end()) {
                ExceptionUtils::runtime(TAG, "layer " + layer.mId + ": source " + sourceVal.GetString() + " doesn't exist");
            }
            layer.mSource = sourceVal.GetString();
        }

        // 3. Parse source-layer
        if (value.HasMember("source-layer")) {
            const rapidjson::Value &sourceLayerVal = value["source-layer"];
            if (!sourceLayerVal.IsString()) {
                ExceptionUtils::runtime(TAG, "layer " + layer.mId + ": source-layer must be a string");
            }
            layer.mSourceLayer = sourceLayerVal.GetString();
        }

        // 4. Parse filter
        //TODO

        // 5. Parse minzoom
        //TODO

        // 6. Parse maxzoom
        //TODO

        // 7. Parse layout
        //TODO

        // 8. Parse paints
        //TODO

        return std::move(layer);
    }

    void StyleParser::parseTileJson(const rapidjson::Value &value, Source& source) {
        // 1. Parse tiles
        if (!value.HasMember("tiles")) {
            ExceptionUtils::runtime(TAG, "tileJSON: 'tiles' is missing");
        }
        const auto& tilesValue = value["tiles"];
        if (!tilesValue.IsArray()) {
            ExceptionUtils::runtime(TAG, "tileJSON: 'tiles' must be an array");
        }
        for (auto it = tilesValue.Begin(); it != tilesValue.End(); ++it) {
            if (!it->IsString()) {
                ExceptionUtils::runtime(TAG, "tileJSON: 'tiles' members must be a string");
            }
            source.mTiles.push_back(it->GetString());
        }

        auto& uri = source.mTiles[0];
        auto protocol = uri.substr(0, uri.find("://"));
        if (protocol == "file") {
            // make the path relative to the style dir
            std::string dir = mFile.substr(0, mFile.rfind("/"));
            uri.insert(uri.find("://") + 3, dir + "/");
            source.mTileProvider = std::make_shared<FileTileProvider>(FileTileProvider(uri));
        } else {
            ExceptionUtils::runtime(TAG, "Invalid protocol: " + protocol);
        }

        // 2. Parse minzoom
        //TODO

        // 3. Parse maxzoom
        if (value.HasMember("maxzoom")) {
            const auto& maxzoomValue = value["maxzoom"];
            if (!maxzoomValue.IsInt()) {
                ExceptionUtils::runtime(TAG, "tileJSON: 'maxzoom' must be an integer");
            }
            source.mMaxZoom = (U8) maxzoomValue.GetInt();
        }


        //TODO parse other values...
    }

}

