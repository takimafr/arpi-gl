#ifndef ARPIGL_SHAPEUTILS_HPP
#define ARPIGL_SHAPEUTILS_HPP

#include <glm/glm.hpp>
#include <vector>
#include <common/Types.hpp>
#include "Triangle.hpp"
#include "shape/ShapeTypes.hpp"

namespace dma {

    class GeometryUtils {
    public:

        /**
         * Extrudes the positions along the Y axis.
         * @param polygon the original vertex positions going to be updated
         * @param indices the original triangles indices going to be updated
         */
        static void extrude(std::vector<glm::vec3>& positions, std::vector<U32>& indices, float height);

        /**
         * Merges closed points and build an index for the triangles.
         * @param triangles the triangles to index
         * @param positions the output merged positions
         * @param indices the output triangle indices
         */
        static void index(const std::vector<glm::vec3>& triangles, std::vector<glm::vec3>& positions, std::vector<U32>& indices);

        /**
         * Scales the points of a Polygon.
         * @param polygon the polygon to scale
         * @param scaleX the x scale factor
         * @param scaleY the y scale factor
         */
        static void scale(Polygon &polygon, float scaleX, float scaleY);

    };

}


#endif //ARPIGL_SHAPEUTILS_HPP
