#include "geo/tile/TileFactory.hpp"
#include "geo/tile/Tile.hpp"

#define TILE_MATERIAL "tile"

namespace dma {


    TileFactory::TileFactory(ResourceManager &resourceManager, LatLng& geoSceneOrigin) :
            mResourceManager(resourceManager),
            mGeoSceneOrigin(geoSceneOrigin)
    {}

    std::shared_ptr<Tile> TileFactory::create() {
        std::shared_ptr<Quad> quad = mResourceManager.createQuad(1.0f, 1.0f);
        std::shared_ptr<Material> mat = mResourceManager.createMaterial(TILE_MATERIAL); //material with default tile texture

        std::shared_ptr<Tile> tile = std::make_shared<Tile>();
        auto geoEntity = std::make_shared<GeoEntity>(quad, mat, mGeoSceneOrigin);

        geoEntity->pitch(-90.0f);
        geoEntity->setScale(quad->getScale());

        tile->mGeoEntity = geoEntity;
        tile->mQuad = quad;

        return tile;
    }

    std::shared_ptr<Tile> TileFactory::create(int x, int y, int z) {
        auto tile = create();
        tile->xyz(x, y, z);
        return tile;
    }




}
