#include <engine/geo/GeoEntity.hpp>

namespace dma {
    namespace geo {

        GeoEntity::GeoEntity(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material,
                             GeoSceneManager &geoSceneManager) :
                Entity(mesh, material),
                mGeoSceneManager(geoSceneManager)
        {}

        void GeoEntity::setCoords(const dma::LatLngAlt &coords) {
            mCoords = coords;
            setPosition(mGeoSceneManager.mapPosition(coords));
        }


    }
}

