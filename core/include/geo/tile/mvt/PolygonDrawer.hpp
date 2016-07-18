#ifndef ARPIGL_POLYGONDRAWER_HPP
#define ARPIGL_POLYGONDRAWER_HPP

#include <common/Types.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <shape/ShapeTypes.hpp>

namespace dma {
    class PolygonDrawer {

    public:

        PolygonDrawer();

        void moveTo(int dx, int dy);
        void lineTo(int dx, int dy);
        void closePath();

        /**
         * Move the backed polygons.
         * Note that the PolygonDrawer become UNUSABLE after this call,
         * you MUST instantiate a new one if you need to draw another multi-polygon
         */
        std::vector<Polygon> polygons();

    private:
        glm::vec2 mPos;
        Ring mCurrentRing;
        std::vector<Polygon> mPolygons;
    };
}


#endif //ARPIGL_POLYGONDRAWER_HPP
