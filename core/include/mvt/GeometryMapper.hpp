#ifndef ARPIGL_GEOMETRYMAPPER_HPP
#define ARPIGL_GEOMETRYMAPPER_HPP

#include <geo/vector_tile.hpp>
#include <glm/glm.hpp>
#include <shape/ShapeTypes.hpp>

namespace dma {

    class GeometryMapper {
    public:
        static std::vector<Polygon> polygons(google::protobuf::RepeatedField<google::protobuf::uint32> geometries);
    };
}

#endif //ARPIGL_GEOMETRYMAPPER_HPP
