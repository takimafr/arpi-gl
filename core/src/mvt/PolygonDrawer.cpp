#include "mvt/PolygonDrawer.hpp"

namespace {
    /**
     * Signed area of polygon using Shoelace formula
     * http://en.wikipedia.org/wiki/Shoelace_formula
     */
    float area(std::vector<glm::vec2>& polygon) {
        unsigned int n = polygon.size();
        float area = 0.0f;
        for (unsigned i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            area += polygon[i].x * polygon[j].y;
            area -= polygon[j].x * polygon[i].y;
        }
        return area / 2.0f;
    }
}

namespace dma {

    PolygonDrawer::PolygonDrawer() {
    }

    void PolygonDrawer::moveTo(int dx, int dy) {
        mPos.x += dx;
        mPos.y += dy;
    }

    void PolygonDrawer::lineTo(int dx, int dy) {
        if (mCurrentRing.empty()) {
            // It is the first segment: add the first point
            mCurrentRing.push_back(mPos);
        }
        mPos.x += dx;
        mPos.y += dy;
        mCurrentRing.push_back(mPos);
    }

    void PolygonDrawer::closePath() {
        // Note: closePath do not change the cursor position.
        if (area(mCurrentRing) >= 0) {
            // It is a new outer ring
            mPolygons.push_back(Polygon());
        }
        mPolygons.back().push_back(std::move(mCurrentRing));
        mCurrentRing = std::vector<glm::vec2>();
    }

    std::vector<Polygon> PolygonDrawer::polygons() {
        return std::move(mPolygons);
    }

}


