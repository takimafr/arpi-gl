#ifndef ARPIGL_GEOENTITY_HPP
#define ARPIGL_GEOENTITY_HPP

#include <engine/Entity.hpp>
#include "LatLngAlt.hpp"
#include "geo/GeoSceneManager.hpp"
#include "geo/OriginListener.hpp"


namespace dma {
    class GeoEntity : public Entity, public OriginListener {

    public:
        GeoEntity(const std::shared_ptr<Mesh> &mesh,
                  const std::shared_ptr<Material> &material);

        GeoEntity(const std::string& sid, const std::shared_ptr<Mesh> &mesh,
                  const std::shared_ptr<Material> &material);

        inline std::string& getSid() {
            return mSid;
        }

        inline LatLngAlt &getCoords() { return mCoords; }

        virtual void setCoords(const LatLngAlt &coords);

        virtual void setCoords(const LatLng &coords);

        virtual void onOriginChanged(const LatLng& newOrigin) override;

    protected:
        std::string mSid;
        LatLngAlt mCoords;
        LatLng mGeoSceneOrigin;
    };
}

#endif //ARPIGL_GEOENTITY_HPP
