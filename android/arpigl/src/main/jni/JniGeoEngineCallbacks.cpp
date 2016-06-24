/*
 * Copyright (C) 2015  eBusiness Information
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <string>

#include "JniGeoEngineCallbacks.hpp"
#include "utils/Log.hpp"

#define TAG = "JniGeoEngineCallbacks";

namespace dma {
    namespace geo {



        JniGeoEngineCallbacks::JniGeoEngineCallbacks(JavaVM* javaVM, jobject listener,
                                                     jmethodID onTileRequest,
                                                     jmethodID onPoiSelected,
                                                     jmethodID onPoiDeselected) :
                mJavaVM(javaVM),
                mListener(listener),
                mOnTileRequest(onTileRequest),
                mOnPoiSelected(onPoiSelected),
                mOnPoiDeselected(onPoiDeselected)
        {

        }


        JniGeoEngineCallbacks::~JniGeoEngineCallbacks() {
        }



        void JniGeoEngineCallbacks::onTileRequest(int x, int y, int z) {

            JNIEnv* env;
            mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
            assert(env != nullptr); // should be called from the already attached OpenGL thread

            env->CallVoidMethod(mListener, mOnTileRequest, x, y, z);

            //TODO
            //if (env->ExceptionCheck()) {
            //    env->ExceptionDescribe();
            //}
        }



        void JniGeoEngineCallbacks::onPoiSelected(const std::string &sid) {

            JNIEnv* env;
            mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
            assert(env != nullptr); // should be called from the already attached OpenGL thread

            jstring jsid = env->NewStringUTF(sid.c_str());
            env->CallVoidMethod(mListener, mOnPoiSelected, jsid);

            //TODO
            //if (env->ExceptionCheck()) {
            //    env->ExceptionDescribe();
            //}
        }



        void JniGeoEngineCallbacks::onPoiDeselected(const std::string &sid) {

            JNIEnv* env;
            mJavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
            assert(env != nullptr); // should be called from the already attached OpenGL thread

            jstring jsid = env->NewStringUTF(sid.c_str());
            env->CallVoidMethod(mListener, mOnPoiDeselected, jsid);

            //TODO
            //if (env->ExceptionCheck()) {
            //    env->ExceptionDescribe();
            //}
        }

    } /* namespace geo */
} /* namespace dma */