#include <algorithm>
#include "shape/GeometryUtils.hpp"

#define EPSILON 0.00001f

namespace dma {

    void GeometryUtils::extrude(std::vector<glm::vec3> &positions, std::vector<U32>& indices, float height) {
        assert(indices.size() % 3 == 0);
        // 0. Create the triangle list
        std::vector<Triangle> triangles;
        for (U32 i = 0; i < indices.size(); i += 3) {
            triangles.push_back(Triangle(indices[i+2], indices[i+1], indices[i+0]));
        }

        // The original vertex and triangle count
        unsigned long n = positions.size();
        unsigned long m = triangles.size();

        // 1. Generate the extruded positions and add them to the vertex list
        std::vector<glm::vec3> extrudedVertices;
        extrudedVertices.reserve(n);
        for (auto v : positions) {
            extrudedVertices.push_back(glm::vec3(v.x, height, v.z));
        }
        positions.insert(positions.end(), extrudedVertices.begin(), extrudedVertices.end());

        // 2. Create the boundary for the original mesh
        std::vector<Edge> edges;
        edges.reserve(3 * m);
        for (auto& t : triangles) {
            for (auto& e : t.edges()) {
                edges.push_back(e);
            }
        }
        // Remove duplicate edges
        std::vector<Edge> boundary;
        boundary.reserve(3 * m);
        for (auto& e : edges) {
            if (std::count(edges.begin(), edges.end(), e) == 1) {
                boundary.push_back(e);
            }
        }

        // 3. Create the extruded face
        std::vector<Triangle> extrudedTriangles;
        extrudedTriangles.reserve(m);
        for (auto& t : triangles) {
            indices.push_back(t.a + n);
            indices.push_back(t.b + n);
            indices.push_back(t.c + n);
        }
        triangles.insert(triangles.end(), extrudedTriangles.begin(), extrudedTriangles.end());

        // 4. Create the sides of the extruded shape
        for (auto& e : boundary) {
            indices.push_back(e.a);
            indices.push_back(e.b);
            indices.push_back(e.b + n);

            indices.push_back(e.b + n);
            indices.push_back(e.a + n);
            indices.push_back(e.a);
        }
    }

    void GeometryUtils::index(std::vector<glm::vec3> &triangles, std::vector<glm::vec3>& positions, std::vector<U32> &indices) {
        for (auto& v : triangles) {
            U32 i = 0;
            for (; i < positions.size(); ++i) {
                if (glm::distance(v, positions[i]) <= EPSILON) {
                    break;
                }
            }
            if (i == positions.size()) {
                positions.push_back(v);
            }
            indices.push_back(i);
        }
    }


}

