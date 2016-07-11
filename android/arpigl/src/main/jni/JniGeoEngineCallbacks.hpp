#ifndef ARPIGL_JNIGEOENGINECALLBACKS_HPP
#define ARPIGL_JNIGEOENGINECALLBACKS_HPP

#include <jni.h>
#include "geo/GeoEngineCallbacks.hpp"

namespace dma {
    class JniGeoEngineCallbacks : public GeoEngineCallbacks {

    public:
        JniGeoEngineCallbacks(JavaVM* javaVM, jobject listener,
                              jmethodID onTileRequest,
                              jmethodID onPoiSelected,
                              jmethodID onPoiDeselected);
        virtual ~JniGeoEngineCallbacks();

        JniGeoEngineCallbacks(const JniGeoEngineCallbacks&) = delete;
        void operator=(const JniGeoEngineCallbacks&) = delete;

        void onTileRequest(int x, int y, int z) override;
        void onPoiSelected(const std::string& sid) override;
        void onPoiDeselected(const std::string& sid) override;

    private :
        JavaVM* mJavaVM;
        jobject mListener;
        jmethodID mOnTileRequest;
        jmethodID mOnPoiSelected;
        jmethodID mOnPoiDeselected;
    };
}

#endif //ARPIGL_JNIGEOENGINECALLBACKS_HPP
