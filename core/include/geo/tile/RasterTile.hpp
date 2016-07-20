#ifndef ARPIGL_RASTERTILE_HPP
#define ARPIGL_RASTERTILE_HPP

#include <geo/GeoEntity.hpp>

namespace dma {
    class RasterTile : public GeoEntity {

    public:
        RasterTile(std::shared_ptr<Quad> quad,
                   std::shared_ptr<Material> material);

        virtual void setCoords(const LatLngAlt &coords) override;

        virtual void setCoords(const LatLng &coords) override;

    private:
        std::shared_ptr<Quad> mQuad;

    };
}


#endif //ARPIGL_RASTERTILE_HPP
