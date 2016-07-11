#include <vector>
#include "shape/Triangle.hpp"

namespace dma {

    Triangle::Triangle(int a, int b, int c) :
            a(a), b(b), c(c)
    {}

    bool Triangle::operator==(const Triangle &other) {
        if (this == &other) return true;

        if (a != other.a) return false;
        if (b != other.b) return false;
        return c == other.c;
    }

    void Triangle::reverse() {
        int tmp = a;
        a = c;
        c = tmp;
    }

    std::vector<Edge> Triangle::edges() {
        std::vector<Edge> edges;
        edges.reserve(3);
        edges.push_back(Edge(a, b));
        edges.push_back(Edge(b, c));
        edges.push_back(Edge(c, a));
        return edges;
    }


}

