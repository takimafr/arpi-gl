#include "mobi_designmyapp_arpigl_engine_Engine_NativeFallthroughEngineListener.h"
#include "JniGeoEngineCallbacks.hpp"
#include "utils/Log.hpp"

#include <cassert>

#ifdef __cplusplus
extern "C" {
#endif

#define TAG "jni_NativeFallthroughEngineListener"

using namespace dma;
using namespace dma::geo;


/*
 * Class:     mobi_designmyapp_arpigl_engine_Engine_NativeFallthroughEngineListener
 * Method:    newEngineListener
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_00024NativeFallthroughEngineListener_newEngineListener
        (JNIEnv* env, jobject caller)
{
    // Get reference to the JVM
    JavaVM* jvm;
    int status = env->GetJavaVM(&jvm);
    assert(status == 0);
    if (status != 0) {
        Log::error(TAG, "cannot get JVM pointer");
        exit(-1);
    }

    // Get java class ref
    jclass javaclass = env->FindClass("mobi/designmyapp/arpigl/engine/Engine$NativeFallthroughEngineListener");
    assert(javaclass != nullptr);

    // Get method ids from Java
    jmethodID onTileRequest = env->GetMethodID(javaclass, "onTileRequest", "(III)V");
    assert(onTileRequest != 0);
    if (onTileRequest == 0) {
        Log::error(TAG, "Cannot get JNI Method with signature onTileRequest((III)V)");
        exit(-1);
    }

    jmethodID onPoiSelected = env->GetMethodID(javaclass, "onPoiSelected", "(Ljava/lang/String;)V");
    assert(onPoiSelected != 0);
    if (onPoiSelected == 0) {
        Log::error(TAG, "Cannot get JNI Method with signature onPoiSelected((Ljava/lang/String;)V)");
        exit(-1);
    }

    jmethodID onPoiDeselected = env->GetMethodID(javaclass, "onPoiDeselected", "(Ljava/lang/String;)V");
    assert(onPoiDeselected != 0);
    if (onPoiDeselected == 0) {
        Log::error(TAG, "Cannot get JNI Method with signature onPoiDeselected((Ljava/lang/String;)V)");
        exit(-1);
    }

    // Convert local to global reference
    caller = env->NewGlobalRef(caller);
    JniGeoEngineCallbacks* callbacks = new JniGeoEngineCallbacks(jvm, caller, onTileRequest, onPoiSelected, onPoiDeselected);
    return (long)callbacks;
}



/*
 * Class:     mobi_designmyapp_arpigl_engine_Engine_NativeFallthroughEngineListener
 * Method:    freeEngineListener
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mobi_designmyapp_arpigl_engine_Engine_00024NativeFallthroughEngineListener_freeEngineListener
        (JNIEnv* env, jobject caller, jlong addr)
{
    env->DeleteGlobalRef(caller);
    delete ((JniGeoEngineCallbacks*) addr);
}

#ifdef __cplusplus
}
#endif