#include <jni.h>
#include <cassert>
#include <cstdlib>

#include "utils/Log.hpp"
#include "JniGeoEngineCallbacks.hpp"

#ifdef __cplusplus
extern "C" {
#endif

using namespace dma;
using namespace dma::geo;

#define TAG "jni_NativeFallthroughEngineListener"

JNIEXPORT jlong JNICALL
Java_mobi_designmyapp_arpigl_engine_NativeFallthroughEngineListener_newEngineListener(
        JNIEnv *env, jobject instance) {

    // Get reference to the JVM
    JavaVM* jvm;
    int status = env->GetJavaVM(&jvm);
    assert(status == 0);
    if (status != 0) {
        Log::error(TAG, "cannot get JVM pointer");
        exit(-1);
    }

    // Get java class ref
    jclass javaclass = env->FindClass("mobi/designmyapp/arpigl/engine/NativeFallthroughEngineListener");
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
    instance = env->NewGlobalRef(instance);
    JniGeoEngineCallbacks* callbacks = new JniGeoEngineCallbacks(jvm, instance, onTileRequest, onPoiSelected, onPoiDeselected);
    return (long)callbacks;
}

JNIEXPORT void JNICALL
Java_mobi_designmyapp_arpigl_engine_NativeFallthroughEngineListener_freeEngineListener(
        JNIEnv *env, jobject instance, jlong nativeAddr) {
    env->DeleteGlobalRef(instance);
    delete ((JniGeoEngineCallbacks*) nativeAddr);
}

#ifdef __cplusplus
}
#endif
