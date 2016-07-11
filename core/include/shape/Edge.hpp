#ifndef ARPIGL_EDGE_HPP
#define ARPIGL_EDGE_HPP

namespace dma {
    class Edge {
    public:

        Edge(int a, int b) : a(a), b(b) {}

        inline bool operator==(const Edge& other) {
            if (this == &other) return true;
            return (a == other.a && b == other.b) || (a == other.b && b == other.a);
        }

        inline bool operator!=(const Edge& other) { return !(*this == other); }

    public:
        const int a;
        const int b;
    };
}


#endif //ARPIGL_EDGE_HPP
