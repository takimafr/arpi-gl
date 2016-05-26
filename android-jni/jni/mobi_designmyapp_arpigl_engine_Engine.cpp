#include "mobi_designmyapp_arpigl_engine_Engine.h"
#include "engine/geo/GeoEngine.hpp"

#ifdef __cplusplus
extern "C" {
#endif

using namespace dma;
using namespace dma::geo;

#define ENGINE(addr) ((GeoEngine*) addr)


//------------------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_newEngine
        (JNIEnv* env, jobject caller, jstring rootDir)
{
    const char* nativeString = env->GetStringUTFChars(rootDir, 0);
    const std::string rootDirStr(nativeString);
    GeoEngine* engine = new GeoEngine(rootDirStr);
    env->ReleaseStringUTFChars(rootDir, nativeString);
    return (jlong)((long) engine);
}


//------------------------------------------------------------------------------------
JNIEXPORT jlong JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_freeEngine
        (JNIEnv* env, jobject caller, jlong addr)
{
    delete ENGINE(addr);
    return (jlong)NULL;
}


//------------------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_init
        (JNIEnv* env, jobject caller, jlong addr)
{
    return ENGINE(addr)->init();
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_refresh
        (JNIEnv* env, jobject caller, jlong addr)
{
    ENGINE(addr)->refresh();
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_unload
        (JNIEnv* env, jobject caller, jlong addr)
{
    ENGINE(addr)->unload();
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_wipe
        (JNIEnv* env, jobject caller, jlong addr)
{
    ENGINE(addr)->wipe();
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_step
        (JNIEnv* env, jobject caller, jlong addr)
{
    ENGINE(addr)->step();
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_addPoi
(JNIEnv* env, jobject caller, jlong addr,
        jstring jsid, jstring jshape, jstring jicon,
        jfloat jr, jfloat jg, jfloat jb,
        jdouble jlat, jdouble jlng, jdouble jalt,
        jboolean animated)
{
    const char* csid = env->GetStringUTFChars(jsid, 0);
    const std::string sid(csid);
    env->ReleaseStringUTFChars(jsid, csid);

    const char* cshape = env->GetStringUTFChars(jshape, 0);
    const std::string shape(cshape);
    env->ReleaseStringUTFChars(jshape, cshape);


    std::string icon = "";
    // icon is optional.
    if (jicon) {
        const char* cicon = env->GetStringUTFChars(jicon, 0);
        icon = std::string(cicon);
        env->ReleaseStringUTFChars(jicon, cicon);
    }

    const Color color = Color((float)jr, (float)jg, (float)jb);
    double lat = (double) jlat;
    double lng = (double) jlng;
    double alt = (double) jalt;

    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    PoiFactory& poiFactory = engine->getPoiFactory();
    // Post message
    engine->post([&geoSceneManager, &poiFactory, sid, shape, icon, color, lat, lng, alt, animated]() {
        std::shared_ptr<Poi> poi = poiFactory.builder()
            .sid(sid)
            .shape(shape)
            .color(color)
            .icon(icon)
            .animation(animated)
            .build();
        poi->setPosition(lat, lng, alt);
        geoSceneManager.addPoi(poi);
    });
}

//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_removePoi
        (JNIEnv* env, jobject caller, jlong addr, jstring jsid)
{
    const char* csid = env->GetStringUTFChars(jsid, 0);
    const std::string sid(csid);
    env->ReleaseStringUTFChars(jsid, csid);

    // post message
    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, sid]() {
        geoSceneManager.removePoi(sid);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setPoiPosition
        (JNIEnv* env, jobject caller, jlong addr, jstring jsid, jdouble jlat, jdouble jlng, jdouble jalt)
{
    const char* csid = env->GetStringUTFChars(jsid, 0);
    const std::string sid(csid);
    env->ReleaseStringUTFChars(jsid, csid);

    double lat = (double) jlat;
    double lng = (double) jlng;
    double alt = (double) jalt;

    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, sid, lat, lng, alt]() {
        std::shared_ptr<Poi> poi = geoSceneManager.getPoi(sid);
        if (poi != nullptr) {
            poi->setPosition(lat, lng, alt);
        }
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setPoiColor
        (JNIEnv* env, jobject caller, jlong addr, jstring jsid, jfloat jr, jfloat jg, jfloat jb)
{
    const char* csid = env->GetStringUTFChars(jsid, 0);
    const std::string sid(csid);
    env->ReleaseStringUTFChars(jsid, csid);

    const Color color = Color((float)jr, (float)jg, (float)jb);

    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, sid, color]() {
        std::shared_ptr<Poi> poi = geoSceneManager.getPoi(sid);
        if (poi != nullptr) {
            poi->setColor(color);
        }
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_isInit
        (JNIEnv* env, jobject caller, jlong addr)
{
    return ENGINE(addr)->isInit();
}


//------------------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_isAbleToDraw
        (JNIEnv* env, jobject caller, jlong addr)
{
    return ENGINE(addr)->isAbleToDraw();
}


//------------------------------------------------------------------------------------
JNIEXPORT jboolean JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_hasPoi
        (JNIEnv* env, jobject caller, jlong addr, jstring jsid)
{
    const char* csid = env->GetStringUTFChars(jsid, 0);
    const std::string sid(csid);
    env->ReleaseStringUTFChars(jsid, csid);
    return (jboolean) ENGINE(addr)->getGeoSceneManager().hasPoi(sid); // must be called from OpenGL thread
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setSkyBox
        (JNIEnv* env, jobject caller, jlong addr, jstring jsid)
{
    const char *csid = env->GetStringUTFChars(jsid, 0);
    const std::string sid(csid);
    env->ReleaseStringUTFChars(jsid, csid);

    GeoEngine* engine = ENGINE(addr);
    Scene& scene = engine->getGeoSceneManager().getScene();
    engine->post([&scene, sid]() {
        scene.setSkyBox(sid);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setSkyBoxEnabled
        (JNIEnv* env, jobject caller, jlong addr, jboolean enabled)
{
    GeoEngine* engine = ENGINE(addr);
    Scene& scene = engine->getGeoSceneManager().getScene();
    engine->post([&scene, enabled]() {
        scene.setSkyBoxEnabled(enabled);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setSurfaceSize
        (JNIEnv* env, jobject caller, jlong addr, jint width, jint height)
{
    ENGINE(addr)->setSurfaceSize(width, height); // must be called from OpenGL thread
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setEngineListener
        (JNIEnv* env, jobject caller, jlong addr, jlong callbackAddr)
{
    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, callbackAddr]() {
        geoSceneManager.setCallbacks((GeoEngineCallbacks*)callbackAddr);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setCameraRotation
    (JNIEnv* env, jobject caller, jlong addr, jfloatArray jmatrix)
{
    float* cmatrix = env->GetFloatArrayElements(jmatrix, 0);
    std::shared_ptr<glm::mat4> matrix = std::make_shared<glm::mat4>(glm::make_mat4(cmatrix));
    env->ReleaseFloatArrayElements(jmatrix, cmatrix, 0);

    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, matrix]() {
        geoSceneManager.orientateCamera(matrix);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setCameraPosition2d
        (JNIEnv* env, jobject caller, jlong addr, jdouble jlat, jdouble jlng)
{
    LatLng coords = LatLng((double)jlat, (double)jlng);
    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, coords]() {
        geoSceneManager.placeCamera(coords);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setCameraPosition
        (JNIEnv* env, jobject caller, jlong addr, jdouble jlat, jdouble jlng, jdouble jalt, jboolean janimated)
{
    LatLngAlt coords = LatLngAlt((double)jlat, (double)jlng, (double)jalt);
    bool animated = (bool)janimated;
    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, coords, animated]() {
        if (animated) {
            geoSceneManager.placeCamera(coords, 0.9f, TranslationAnimation::Function::EASE);

        } else {
            geoSceneManager.placeCamera(coords);
        }
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_zoom
    (JNIEnv* env, jobject caller, jlong addr, jfloat joffset)
{
    double offset = (double)joffset;
    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, offset]() {
        geoSceneManager.getScene().getCamera().zoom(offset);
    });
}

//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setOrigin
    (JNIEnv* env, jobject caller, jlong addr, jdouble jlat, jdouble jlng)
{
    double lat = (double)jlat;
    double lng = (double)jlng;

    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, lat, lng]() {
        geoSceneManager.setOrigin(lat, lng);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_notifyTileAvailable
        (JNIEnv* env, jobject caller, jlong addr, jint x, jint y, jint z)
{
    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, x, y, z]() {
        geoSceneManager.notifyTileAvailable(x, y, z);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_setTileNamespace
    (JNIEnv* env, jobject caller, jlong addr, jstring jnamespace)
{
    const char *cnamespace = env->GetStringUTFChars(jnamespace, 0);
    const std::string ns(cnamespace);
    env->ReleaseStringUTFChars(jnamespace, cnamespace);

    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, ns]() {
        geoSceneManager.setTileNamespace(ns);
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_updateTileDiffuseMaps
    (JNIEnv* env, jobject caller, jlong addr)
{
    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager]() {
        geoSceneManager.updateTileDiffuseMaps();
    });
}


//------------------------------------------------------------------------------------
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_selectPoi
    (JNIEnv* env, jobject caller, jlong addr, jint jx, jint jy)
{
    int x = (int) jx;
    int y = (int) jy;
    GeoEngine* engine = ENGINE(addr);
    GeoSceneManager& geoSceneManager = engine->getGeoSceneManager();
    engine->post([&geoSceneManager, x, y]() {
        geoSceneManager.pick(x, y);
    });
}

#ifdef __cplusplus
}
#endif
