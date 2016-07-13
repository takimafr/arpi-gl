#include <shape/GeometryUtils.hpp>
#include <poly2tri/poly2tri.h>
#include "shape/ShapeFactory.hpp"

using namespace std;
using namespace p2t;
using namespace glm;

namespace {

    std::vector<glm::vec3> mapToVec3(const std::vector<Triangle *> &triangles) {
        std::vector<glm::vec3> res;
        res.reserve(triangles.size());
        for (auto t : triangles) {
            for (int i = 0; i < 3; ++i) {
                Point* p = t->GetPoint(i);
                res.push_back(glm::vec3(p->x, 0.0f, p->y));
            }
        }
        return res;
    }

}

namespace dma {

    ShapeFactory::ShapeFactory(ResourceManager& resourceManager) :
            mResourceManager(resourceManager)
    {}

    shared_ptr<Mesh> ShapeFactory::polygon(const Polygon& polygon) {
        vector<vector<Point*>> inputPolygons;
        inputPolygons.reserve(polygon.size());

        float xmin, xmax, ymin, ymax;
        xmin = xmax = polygon[0][0].x;
        ymin = ymax = polygon[0][0].y;


        for (auto& ring : polygon) {
            vector<Point*> inputRing;
            inputRing.reserve(ring.size());
            // Reverse the order for poly2tri
            for (auto it = ring.rbegin(); it != ring.rend(); ++it) {
                auto& p = *it;
                if (p.x < xmin) xmin = p.x;
                if (p.x > xmax) xmax = p.x;
                if (p.y < ymin) ymin = p.y;
                if (p.y > ymax) ymax = p.y;
                inputRing.push_back(new Point(p.x, p.y));
            }
            inputPolygons.push_back(inputRing);
        }

        const auto& outer = inputPolygons[0];

        CDT cdt(outer);

        for (unsigned i = 1; i < inputPolygons.size(); ++i) {
            cdt.AddHole(inputPolygons[i]);
        }

        cdt.Triangulate();

       auto outputTriangles = cdt.GetTriangles();

        // Map to glm::vec3
        std::vector<glm::vec3> triangles = mapToVec3(outputTriangles);

        // Clear resources
        for (auto& ring : inputPolygons) {
            for (auto& p : ring) {
                delete p;
            }
        }

        std::vector<glm::vec3> positions;
        std::vector<U32> indices;

        GeometryUtils::index(triangles, positions, indices);
        GeometryUtils::extrude(positions, indices, 10.f); //TODO parametrize height

        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> flatNormals;
        std::vector<glm::vec3> smoothNormals;
        std::vector<VertexIndices> vertexIndices;
        for (U32 idx : indices) {
            VertexIndices vi;
            vi.p = idx;
            vertexIndices.push_back(vi);
        }

        return mResourceManager.createMesh(positions, uvs, flatNormals, smoothNormals, vertexIndices);
    }

}