#ifndef ARPIGL_LAYER_HPP
#define ARPIGL_LAYER_HPP

#include <string>
#include <map>
#include "geo/tile/Source.hpp"

namespace dma {
    class Layer {

        friend class StyleParser;

    public:

        enum Type { EXTRUDE, BACKGROUND, FILL, LINE, SYMBOL, RASTER, CIRCLE, UNKNOWN };

        static Type type(const std::string& type);

        inline Type getType() {
            return mType;
        }

        inline const std::string& getSource() {
            return mSource;
        }

    private:
        std::string mId;
        std::string mSource;
        std::string mSourceLayer;
        Type mType;
        std::map<std::string, std::string> mLayout;
        std::map<std::string, std::string> mPaint;
    };
}


#endif //ARPIGL_LAYER_HPP
