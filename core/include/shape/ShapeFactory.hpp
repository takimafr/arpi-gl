#ifndef ARPIGL_SHAPEFACTORY_HPP
#define ARPIGL_SHAPEFACTORY_HPP


#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <resource/Mesh.hpp>
#include <resource/ResourceManager.hpp>
#include "ShapeTypes.hpp"

namespace dma {
    class ShapeFactory {

    public:
        ShapeFactory(ResourceManager& resourceManger);

        std::shared_ptr<Mesh> polygon(const Polygon& polygon);

    private:
        ResourceManager& mResourceManager;
    };
}


#endif //ARPIGL_SHAPEFACTORY_HPP
