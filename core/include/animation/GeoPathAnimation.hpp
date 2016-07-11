#ifndef ARPIGL_GEOPATHANIMATION_HPP
#define ARPIGL_GEOPATHANIMATION_HPP

#include <geo/LatLngAlt.hpp>
#include <geo/GeoSceneManager.hpp>

namespace dma {

    class GeoPathAnimation : public Animation {

    public:
        GeoPathAnimation(TransformComponent &transformComponent,
                         std::vector<LatLngAlt> &path,
                         float speed, bool loop,
                         GeoSceneManager& geoSceneManager);

        virtual void update(float dt);

    private:
        std::vector<float> mDurations;
        std::vector<LatLngAlt> mPath;
        GeoSceneManager& mGeoSceneManager;
    };
}

#endif //ARPIGL_GEOPATHANIMATION_HPP
