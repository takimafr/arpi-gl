#ifndef ARPIGL_GEOENTITY_HPP
#define ARPIGL_GEOENTITY_HPP

#include <engine/Entity.hpp>
#include "LatLngAlt.hpp"
#include "geo/GeoSceneManager.hpp"


namespace dma {
    class GeoEntity : public Entity {

    public:
        GeoEntity(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material,
                  GeoSceneManager &geoSceneManager);

        inline LatLngAlt &getCoords() { return mCoords; }

        virtual void setCoords(const LatLngAlt &coords);

    protected:
        LatLngAlt mCoords;
        GeoSceneManager &mGeoSceneManager;
    };
}

#endif //ARPIGL_GEOENTITY_HPP
