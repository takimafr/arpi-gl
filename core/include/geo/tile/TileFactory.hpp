#ifndef ARPIGL_TILEFACTORY_HPP
#define ARPIGL_TILEFACTORY_HPP


#include <resource/ResourceManager.hpp>
#include <geo/LatLng.hpp>

namespace dma {

    class Tile;
    class GeoSceneManager;

    class TileFactory {

    public:
        TileFactory(ResourceManager& resourceManager, LatLng& geoSceneOrigin);

        std::shared_ptr<Tile> create();
        std::shared_ptr<Tile> create(int x, int y, int z);

    private:
        ResourceManager& mResourceManager;
        LatLng& mGeoSceneOrigin;
    };
}


#endif //ARPIGL_TILEFACTORY_HPP
