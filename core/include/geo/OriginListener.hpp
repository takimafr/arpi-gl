#ifndef ARPIGL_ORIGINSHIFTLISTENER_HPP
#define ARPIGL_ORIGINSHIFTLISTENER_HPP

namespace dma {

    class OriginListener {
    public:
        virtual void onOriginChanged(const LatLng & newOrigin) = 0;
    };
}

#endif //ARPIGL_ORIGINSHIFTLISTENER_HPP
