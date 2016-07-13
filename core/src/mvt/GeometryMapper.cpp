#include <common/Types.hpp>
#include <mvt/Command.hpp>
#include <mvt/PolygonDrawer.hpp>
#include <utils/Log.hpp>
#include <utils/ExceptionUtils.hpp>
#include "mvt/GeometryMapper.hpp"

#define TAG "FeatureMapper"

namespace {
    int zigzag(google::protobuf::uint32 param) {
        return ((param >> 1) ^ (-(param & 1)));
    }
}

namespace dma {

    std::vector<Polygon> GeometryMapper::polygons(google::protobuf::RepeatedField<google::protobuf::uint32> geometries) {
        PolygonDrawer drawer;

        for (auto it = geometries.begin(); it != geometries.end(); ++it) {
            U32 command = *it;

            U32 id = command & 0x7;
            U32 count = command >> 3;

            switch (id) {
                case Command::MOVE_TO:
                    for (unsigned i = 0; i < count; ++i) {
                        int dx = zigzag(*(++it));
                        int dy = zigzag(*(++it));
                        drawer.moveTo(dx, dy);
                    }
                    break;

                case Command::LINE_TO:
                    for (unsigned i = 0; i < count; ++i) {
                        int dx = zigzag(*(++it));
                        int dy = zigzag(*(++it));
                        drawer.lineTo(dx, dy);
                    }
                    break;

                case Command::CLOSE_PATH:
                    drawer.closePath();
                    break;
                default:
                    ExceptionUtils::runtime(TAG, "Invalid command id: " + id);
            }
        }
        return std::move(drawer.polygons());
    }

}
