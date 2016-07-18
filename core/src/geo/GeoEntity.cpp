#include <geo/GeoEntity.hpp>
#include <utils/GeoUtils.hpp>

namespace dma {

    GeoEntity::GeoEntity(const std::shared_ptr<Mesh> &mesh,
                         const std::shared_ptr<Material> &material,
                         LatLng& geoSceneOrigin):
            Entity(mesh, material),
            mGeoSceneOrigin(geoSceneOrigin)
    {}

    void GeoEntity::setCoords(const LatLngAlt &coords) {
        mCoords = coords;
        LatLngAlt origin(mGeoSceneOrigin.lat, mGeoSceneOrigin.lng, 0.0f);
        setPosition(GeoUtils::vector(origin, coords));
    }

    void GeoEntity::setCoords(const LatLng &coords) {
        setCoords(LatLngAlt(coords.lat, coords.lng, getPosition().y));
    }


}

