LOCAL_PATH := $(call my-dir)
$(call import-add-path, $(LOCAL_PATH)/ndk-modules)
include $(CLEAR_VARS)

ROOT_PATH := ../../../../..

ANIMATION_CPP := \
     $(ROOT_PATH)/core/src/animation/Animation.cpp      		\
     $(ROOT_PATH)/core/src/animation/AnimationComponent.cpp   	\
     $(ROOT_PATH)/core/src/animation/AnimationSystem.cpp		\
     $(ROOT_PATH)/core/src/animation/GeoPathAnimation.cpp		\
     $(ROOT_PATH)/core/src/animation/PathAnimation.cpp		\
     $(ROOT_PATH)/core/src/animation/RotationAnimation.cpp		\
     $(ROOT_PATH)/core/src/animation/SlerpAnimation.cpp       	\
     $(ROOT_PATH)/core/src/animation/TranslationAnimation.cpp 	

COMMON_CPP := \
    $(ROOT_PATH)/core/src/common/Timer.cpp

ENGINE_CPP :=  \
    $(ROOT_PATH)/core/src/engine/Engine.cpp                \
    $(ROOT_PATH)/core/src/engine/Entity.cpp                \
    $(ROOT_PATH)/core/src/engine/Scene.cpp                 \
    $(ROOT_PATH)/core/src/engine/TransformComponent.cpp

GEO_CPP := \
    $(ROOT_PATH)/core/src/geo/GeoEngine.cpp          \
    $(ROOT_PATH)/core/src/geo/GeoEngineCallbacks.cpp \
    $(ROOT_PATH)/core/src/geo/GeoEntity.cpp          \
    $(ROOT_PATH)/core/src/geo/Poi.cpp				         \
    $(ROOT_PATH)/core/src/geo/PoiFactory.cpp         \
    $(ROOT_PATH)/core/src/geo/GeoSceneManager.cpp    \
    $(ROOT_PATH)/core/src/geo/Tile.cpp               \
    $(ROOT_PATH)/core/src/geo/TileMap.cpp            \
    $(ROOT_PATH)/core/src/geo/vector_tile.cpp

ASYNC_CPP := \
    $(ROOT_PATH)/core/src/async/TaskScheduler.cpp

RENDERING_CPP := \
    $(ROOT_PATH)/core/src/rendering/BoundingSphere.cpp  \
    $(ROOT_PATH)/core/src/rendering/Camera.cpp					\
    $(ROOT_PATH)/core/src/rendering/FlyThroughCamera.cpp        \
    $(ROOT_PATH)/core/src/rendering/Frustum.cpp                 \
    $(ROOT_PATH)/core/src/rendering/HUDSystem.cpp               \
    $(ROOT_PATH)/core/src/rendering/HUDElement.cpp              \
    $(ROOT_PATH)/core/src/rendering/IndexBuffer.cpp       		\
    $(ROOT_PATH)/core/src/rendering/Plane.cpp             		\
    $(ROOT_PATH)/core/src/rendering/RenderingComponent.cpp    	\
    $(ROOT_PATH)/core/src/rendering/RenderingEngine.cpp   		\
    $(ROOT_PATH)/core/src/rendering/RenderingPackage.cpp  		\
    $(ROOT_PATH)/core/src/rendering/SkyBox.cpp  		        \
    $(ROOT_PATH)/core/src/rendering/Vertex.cpp            		\
    $(ROOT_PATH)/core/src/rendering/VertexBuffer.cpp


RESOURCE_CPP :=  \
   $(ROOT_PATH)/core/src/resource/CubeMap.cpp         \
   $(ROOT_PATH)/core/src/resource/CubeMapManager.cpp  \
   $(ROOT_PATH)/core/src/resource/Image.cpp           \
   $(ROOT_PATH)/core/src/resource/Map.cpp             \
   $(ROOT_PATH)/core/src/resource/MapManager.cpp      \
   $(ROOT_PATH)/core/src/resource/Material.cpp        \
   $(ROOT_PATH)/core/src/resource/MaterialManager.cpp \
   $(ROOT_PATH)/core/src/resource/Mesh.cpp            \
   $(ROOT_PATH)/core/src/resource/MeshManager.cpp     \
   $(ROOT_PATH)/core/src/resource/Pass.cpp            \
   $(ROOT_PATH)/core/src/resource/Quad.cpp            \
   $(ROOT_PATH)/core/src/resource/QuadFactory.cpp     \
   $(ROOT_PATH)/core/src/resource/ResourceManager.cpp \
   $(ROOT_PATH)/core/src/resource/ShaderManager.cpp   \
   $(ROOT_PATH)/core/src/resource/ShaderProgram.cpp   \
   $(ROOT_PATH)/core/src/resource/Texture.cpp         \
   $(ROOT_PATH)/core/src/resource/TrackFactory.cpp


UTILS_CPP := \
   $(ROOT_PATH)/core/src/utils/GeoUtils.cpp           \
   $(ROOT_PATH)/core/src/utils/GeoSceneReader.cpp 		\
   $(ROOT_PATH)/core/src/utils/GLUtils.cpp 				    \
   $(ROOT_PATH)/core/src/utils/MaterialReader.cpp 		\
   $(ROOT_PATH)/core/src/utils/ObjReader.cpp 			    \
   $(ROOT_PATH)/core/src/utils/Utils.cpp 				      \
   utils/Log.cpp


JNI_CPP := \
    JniGeoEngineCallbacks.cpp                      \
    JniEngine.cpp                                  \
    JniNativeFallthroughEngineListener.cpp


LOCAL_MODULE := arpigl
LOCAL_C_INCLUDES := 				\
$(LOCAL_PATH)/$(ROOT_PATH)/core/include    \
$(LOCAL_PATH)/$(ROOT_PATH)/third           \
$(LOCAL_PATH)/$(ROOT_PATH)/third/rapidjson \
$(LOCAL_PATH)/ndk-modules

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_LDLIBS    := -llog -lGLESv2 -lz
LOCAL_STATIC_LIBRARIES := png16 protobuf

LOCAL_SRC_FILES :=   \
    $(ANIMATION_CPP) \
    $(COMMON_CPP)    \
    $(ENGINE_CPP)    \
    $(GEO_CPP)\
    $(JNI_CPP)  	 \
    $(ASYNC_CPP)     \
    $(RENDERING_CPP) \
    $(RESOURCE_CPP)  \
    $(UTILS_CPP)

LOCAL_CFLAGS    		:= -std=c++11 -fexceptions -Wall -Wno-comment -Wno-strict-aliasing -frtti #-Werror
LOCAL_EXPORT_LDLIBS 	:= $(LOCAL_LDLIBS)

ifeq ($(APP_OPTIM),debug)
LOCAL_CFLAGS 			+= -g -DDEBUG -DTRACE -DNWAIT_FOR_GDB -DFPS_PRINT_RATE="5.0"
else
LOCAL_CFLAGS 			+= -O3 -DNDEBUG -DFPS_PRINT_RATE="5.0"
endif

include $(BUILD_SHARED_LIBRARY)
$(call import-module, libprotobuf)
$(call import-module, libpng16)

