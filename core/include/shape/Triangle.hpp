#ifndef ARPIGL_TRIANGLE_HPP
#define ARPIGL_TRIANGLE_HPP

#include "shape/Edge.hpp"

namespace dma {
    class Triangle {
    public:

        Triangle(int a, int b, int c);

        bool operator==(const Triangle& other);
        inline bool operator!=(const Triangle& other) { return !(*this == other); }

        void reverse();

        std::vector<Edge> edges();

        int a;
        int b;
        int c;
    };
}

#endif //ARPIGL_TRIANGLE_HPP
