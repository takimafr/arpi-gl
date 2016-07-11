#ifndef ARPIGL_SHAPEUTILS_HPP
#define ARPIGL_SHAPEUTILS_HPP

#include <glm/glm.hpp>
#include <vector>
#include <common/Types.hpp>
#include "Triangle.hpp"

namespace dma {

    class GeometryUtils {
    public:
        static void extrude(std::vector<glm::vec3>& positions, std::vector<U32>& indices, float height);

        static void index(std::vector<glm::vec3>& triangles, std::vector<glm::vec3>& positions, std::vector<U32>& indices);

    };

}


#endif //ARPIGL_SHAPEUTILS_HPP
