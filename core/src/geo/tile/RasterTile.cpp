#include "geo/tile/RasterTile.hpp"


namespace dma {

    RasterTile::RasterTile(std::shared_ptr<Quad> quad, std::shared_ptr<Material> material) :
        GeoEntity(quad, material),
        mQuad(quad)
    {
        pitch(-90.0f);
        setScale(quad->getScale());
    }

    void RasterTile::setCoords(const LatLngAlt &coords) {
        GeoEntity::setCoords(coords);
        // Shifts the quad position since its origin is the center
        glm::vec3 pos = getPosition();
        pos.x += mQuad->getWidth() / 2.0f;
        pos.z += mQuad->getHeight() / 2.0f;
        setPosition(pos);
    }

    void RasterTile::setCoords(const LatLng &coords) {
        RasterTile::setCoords(LatLngAlt(coords.lat, coords.lng, getPosition().y));
    }


}

