#include <jni.h>

#include "engine/geo/GeoEngine.hpp"

#ifdef __cplusplus
extern "C" {
#endif

using namespace dma;
using namespace dma::geo;

#define ENGINE(addr) ((GeoEngine*) addr)


JNIEXPORT jlong JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_newEngine(JNIEnv *env, jobject instance,
                                                     jstring rootDir_) {
    const char *rootDir = env->GetStringUTFChars(rootDir_, 0);
    const std::string rootDirStr(rootDir);
    GeoEngine *engine = new GeoEngine(rootDirStr);
    env->ReleaseStringUTFChars(rootDir_, rootDir);
    return (jlong)((long) engine);
}

JNIEXPORT jlong JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_freeEngine(JNIEnv *env, jobject instance,
                                                      jlong nativeInstanceAddr) {
    delete ENGINE(nativeInstanceAddr);
    return (jlong) nullptr;
}

JNIEXPORT jboolean JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_init__J(JNIEnv *env, jobject instance,
                                                   jlong nativeInstanceAddr) {
    return (jboolean) ENGINE(nativeInstanceAddr)->init();
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_refresh__J(JNIEnv *env, jobject instance,
                                                      jlong nativeInstanceAddr) {
    ENGINE(nativeInstanceAddr)->refresh();
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_unload__J(JNIEnv *env, jobject instance,
                                                     jlong nativeInstanceAddr) {
    ENGINE(nativeInstanceAddr)->unload();
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_wipe__J(JNIEnv *env, jobject instance,
                                                   jlong nativeInstanceAddr) {
    ENGINE(nativeInstanceAddr)->wipe();
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_step__J(JNIEnv *env, jobject instance,
                                                   jlong nativeInstanceAddr) {
    ENGINE(nativeInstanceAddr)->step();
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_addPoi__JLjava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2FFFDDDZ(
        JNIEnv *env, jobject instance, jlong nativeInstanceAddr, jstring sid_, jstring shape_,
        jstring icon_, jfloat r, jfloat g, jfloat b, jdouble lat, jdouble lng, jdouble alt,
        jboolean animated) {
    const char *sid = env->GetStringUTFChars(sid_, 0);
    const char *shape = env->GetStringUTFChars(shape_, 0);
    const char *icon = env->GetStringUTFChars(icon_, 0);

//    std::string icon = "";
    // icon is optional.
    const Color color = Color((float)r, (float)g, (float)b);
    const LatLngAlt coords(lat, lng, (float) alt);

    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    PoiFactory& poiFactory = engine->getPoiFactory();
    // Post message
    engine->post([&geoSceneManager, &poiFactory, sid, shape, icon, color, coords, animated]() {
        std::shared_ptr<Poi> poi = poiFactory.builder()
                .sid(sid)
                .shape(shape)
                .color(color)
                .icon(icon)
                .animation(animated)
                .build();
        poi->setCoords(coords);
        geoSceneManager.addGeoEntity(poi->getSid(), poi);
    });

    env->ReleaseStringUTFChars(sid_, sid);
    env->ReleaseStringUTFChars(shape_, shape);
    env->ReleaseStringUTFChars(icon_, icon);
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_removePoi__JLjava_lang_String_2(JNIEnv *env,
                                                                           jobject instance,
                                                                           jlong nativeInstanceAddr,
                                                                           jstring sid_) {
    const char *sid = env->GetStringUTFChars(sid_, 0);
    // post message
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, sid]() {
        geoSceneManager.removeGeoEntity(sid);
    });

    env->ReleaseStringUTFChars(sid_, sid);
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setPoiPosition__JLjava_lang_String_2DDD(JNIEnv *env,
                                                                                   jobject instance,
                                                                                   jlong nativeInstanceAddr,
                                                                                   jstring sid_,
                                                                                   jdouble lat,
                                                                                   jdouble lng,
                                                                                   jdouble alt) {
    const char *sid = env->GetStringUTFChars(sid_, 0);
    const LatLngAlt coords(lat, lng, (float) alt);
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, sid, coords]() {
        std::shared_ptr<GeoEntity> poi = geoSceneManager.getGeoEntity(sid);
        if (poi != nullptr) {
            poi->setCoords(coords);
        }
    });

    env->ReleaseStringUTFChars(sid_, sid);
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setPoiColor__JLjava_lang_String_2FFF(JNIEnv *env,
                                                                                jobject instance,
                                                                                jlong nativeInstanceAddr,
                                                                                jstring sid_,
                                                                                jfloat r, jfloat g,
                                                                                jfloat b) {
    const char *sid = env->GetStringUTFChars(sid_, 0);

    const Color color = Color((float)r, (float)g, (float)b);

    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, sid, color]() {
        std::shared_ptr<GeoEntity> poi = geoSceneManager.getGeoEntity(sid);
        if (poi != nullptr) {
            std::static_pointer_cast<Poi>(poi)->setColor(color);
        }
    });

    env->ReleaseStringUTFChars(sid_, sid);
}

JNIEXPORT jboolean JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_isInit__J(JNIEnv *env, jobject instance,
                                                     jlong nativeInstanceAddr) {
    return (jboolean) ENGINE(nativeInstanceAddr)->isInit();
}

JNIEXPORT jboolean JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_isAbleToDraw__J(JNIEnv *env, jobject instance,
                                                           jlong nativeInstanceAddr) {
    return (jboolean) ENGINE(nativeInstanceAddr)->isAbleToDraw();
}

JNIEXPORT jboolean JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_hasPoi__JLjava_lang_String_2(JNIEnv *env,
                                                                        jobject instance,
                                                                        jlong nativeInstanceAddr,
                                                                        jstring sid_) {
    const char *sid = env->GetStringUTFChars(sid_, 0);
    jboolean res = (jboolean) ENGINE(nativeInstanceAddr)->getGeoSceneManager().hasGeoEntity(sid); // must be called from OpenGL thread
    env->ReleaseStringUTFChars(sid_, sid);
    return res;
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setSkyBox__JLjava_lang_String_2(JNIEnv *env,
                                                                           jobject instance,
                                                                           jlong nativeInstanceAddr,
                                                                           jstring sid_) {
    const char *sid = env->GetStringUTFChars(sid_, 0);
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    Scene& scene = engine->getGeoSceneManager().getScene();
    engine->post([&scene, sid]() {
        scene.setSkyBox(sid);
    });
    env->ReleaseStringUTFChars(sid_, sid);
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setSkyBoxEnabled__JZ(JNIEnv *env, jobject instance,
                                                                jlong nativeInstanceAddr,
                                                                jboolean enabled) {
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    Scene& scene = engine->getGeoSceneManager().getScene();
    engine->post([&scene, enabled]() {
        scene.setSkyBoxEnabled(enabled);
    });

}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setSurfaceSize__JII(JNIEnv *env, jobject instance,
                                                               jlong nativeInstanceAddr, jint width,
                                                               jint height) {
    ENGINE(nativeInstanceAddr)->setSurfaceSize(width, height); // must be called from OpenGL thread
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setEngineListener(JNIEnv *env, jobject instance,
                                                             jlong nativeInstanceAddr,
                                                             jlong nativeCallbackAddr) {
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, nativeCallbackAddr]() {
        geoSceneManager.setCallbacks((GeoEngineCallbacks*)nativeCallbackAddr);
    });
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setCameraRotation__JLfloat_3_093_2(JNIEnv *env,
                                                                              jobject instance,
                                                                              jlong nativeInstanceAddr,
                                                                              jfloatArray rotationMatrix_) {
    jfloat *rotationMatrix = env->GetFloatArrayElements(rotationMatrix_, NULL);
    std::shared_ptr<glm::mat4> matrix = std::make_shared<glm::mat4>(glm::make_mat4(rotationMatrix));

    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, matrix]() {
        geoSceneManager.orientateCamera(matrix);
    });
    env->ReleaseFloatArrayElements(rotationMatrix_, rotationMatrix, 0);
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setCameraPosition2d(JNIEnv *env, jobject instance,
                                                               jlong nativeInstanceAddr,
                                                               jdouble lat, jdouble lng) {
    LatLng coords = LatLng((double)lat, (double)lng);
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, coords]() {
        geoSceneManager.placeCamera(coords);
    });
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setCameraPosition__JDDDZ(JNIEnv *env, jobject instance,
                                                                    jlong nativeInstanceAddr,
                                                                    jdouble lat, jdouble lng,
                                                                    jdouble alt,
                                                                    jboolean animated) {
    LatLngAlt coords = LatLngAlt((double)lat, (double)lng, (float)alt);
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, coords, animated]() {
        if (animated) {
            geoSceneManager.placeCamera(coords, 0.9f, TranslationAnimation::Function::EASE);

        } else {
            geoSceneManager.placeCamera(coords);
        }
    });
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_zoom__JF(JNIEnv *env, jobject instance,
                                                    jlong nativeInstanceAddr, jfloat offset) {
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, offset]() {
        geoSceneManager.getScene().getCamera().zoom(offset);
    });
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setOrigin__JDD(JNIEnv *env, jobject instance,
                                                          jlong nativeInstanceAddr, jdouble lat,
                                                          jdouble lng) {
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, lat, lng]() {
        geoSceneManager.setOrigin(lat, lng);
    });
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_notifyTileAvailable__JIII(JNIEnv *env, jobject instance,
                                                                     jlong nativeInstanceAddr,
                                                                     jint x, jint y, jint z) {
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, x, y, z]() {
        geoSceneManager.notifyTileAvailable(x, y, z);
    });
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_setTileNamespace__JLjava_lang_String_2(JNIEnv *env,
                                                                                  jobject instance,
                                                                                  jlong nativeInstanceAddr,
                                                                                  jstring ns_) {
    const char *ns = env->GetStringUTFChars(ns_, 0);
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, ns]() {
        geoSceneManager.setTileNamespace(ns);
    });
    env->ReleaseStringUTFChars(ns_, ns);
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_updateTileDiffuseMaps__J(JNIEnv *env, jobject instance,
                                                                    jlong nativeInstanceAddr) {
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager]() {
        geoSceneManager.updateTileDiffuseMaps();
    });
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_Engine_selectPoi__JII(JNIEnv *env, jobject instance,
                                                          jlong nativeInstanceAddr, jint x,
                                                          jint y) {
    GeoEngine* engine = ENGINE(nativeInstanceAddr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, x, y]() {
        geoSceneManager.pick(x, y);
    });
}

#ifdef __cplusplus
}
#endif
