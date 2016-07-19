#include <geo/GeoEntity.hpp>
#include <utils/GeoUtils.hpp>

namespace dma {

    GeoEntity::GeoEntity(const std::shared_ptr<Mesh> &mesh,
                         const std::shared_ptr<Material> &material):
            GeoEntity("", mesh, material)
    {}

    GeoEntity::GeoEntity(const std::string &sid, const std::shared_ptr<Mesh> &mesh,
                         const std::shared_ptr<Material> &material) :
            Entity(mesh, material),
            mSid(sid)
    {}

    void GeoEntity::setCoords(const LatLngAlt &coords) {
        mCoords = coords;
        LatLngAlt origin(mGeoSceneOrigin.lat, mGeoSceneOrigin.lng, 0.0f);
        setPosition(GeoUtils::vector(origin, coords));
    }

    void GeoEntity::setCoords(const LatLng &coords) {
        setCoords(LatLngAlt(coords.lat, coords.lng, getPosition().y));
    }


    void GeoEntity::onOriginChanged(const LatLng& newOrigin) {
        mGeoSceneOrigin = newOrigin;
        // Update position according to the new origin
        setCoords(mCoords);
    }


}

