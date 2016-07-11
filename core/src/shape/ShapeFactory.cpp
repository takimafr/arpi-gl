#include <clip2tri/clip2tri.h>
#include <shape/GeometryUtils.hpp>
#include "shape/ShapeFactory.hpp"

using namespace std;
using namespace c2t;
using namespace glm;


namespace dma {

    ShapeFactory::ShapeFactory(ResourceManager& resourceManager) :
        mResourceManager(resourceManager)
    {}

    shared_ptr<Mesh> ShapeFactory::polygon(const vector<vector<vec2>>& polygons) {
        vector<vector<Point>> inputPolygons;
        inputPolygons.reserve(polygons.size());

        float xmin, xmax, ymin, ymax;
        xmin = xmax = polygons[0][0].x;
        ymin = ymax = polygons[0][0].y;

        for (auto& poly : polygons) {
            vector<Point> inputPoly(poly.size());
            // Reverse the order for poly2tri
            for (auto it = poly.rbegin(); it != poly.rend(); ++it) {
                auto& p = *it;
                if (p.x < xmin) xmin = p.x;
                if (p.x > xmax) xmax = p.x;
                if (p.y < ymin) ymin = p.y;
                if (p.y > ymax) ymax = p.y;
                inputPoly.push_back(Point(p.x, p.y));
            }
            inputPolygons.push_back(inputPoly);
        }

        vector<Point> bounds;
        bounds.push_back(Point(xmin, ymin));
        bounds.push_back(Point(xmin, ymax));
        bounds.push_back(Point(xmax, ymax));
        bounds.push_back(Point(xmax, ymin));

        vector<Point> outputTriangles;

        clip2tri clip2tri;
        clip2tri.triangulate(inputPolygons, outputTriangles, bounds);

        // Map to glm::vec3
        std::vector<glm::vec3> triangles;
        triangles.reserve(outputTriangles.size());
        for (auto& p : outputTriangles) {
            triangles.push_back(glm::vec3(p.x, 0.0f, p.y));
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