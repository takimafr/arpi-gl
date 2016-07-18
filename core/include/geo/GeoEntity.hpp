#ifndef ARPIGL_GEOENTITY_HPP
#define ARPIGL_GEOENTITY_HPP

#include <engine/Entity.hpp>
#include "LatLngAlt.hpp"
#include "geo/GeoSceneManager.hpp"


namespace dma {
    class GeoEntity : public Entity {

    public:
        GeoEntity(const std::shared_ptr<Mesh> &mesh,
                  const std::shared_ptr<Material> &material,
                  LatLng& geoSceneOrigin);

        inline LatLngAlt &getCoords() { return mCoords; }

        virtual void setCoords(const LatLngAlt &coords);

        virtual void setCoords(const LatLng &coords);

    protected:
        LatLngAlt mCoords;
        LatLng& mGeoSceneOrigin;
    };
}

#endif //ARPIGL_GEOENTITY_HPP
