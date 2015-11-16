cmake_minimum_required(VERSION 2.8)
project(arpigl)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -pedantic -Werror -Wno-comment -Wno-strict-aliasing -DDEBUG -DTRACE -DFPS_PRINT_RATE=5")

# Setup glfw
set(GLFW_CLIENT_LIBRARY "glesv2" CACHE STRING
        "The client library to use; one of opengl, glesv1 or glesv2")
option(GLFW_USE_EGL "Use EGL for context creation" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

add_subdirectory(third/glfw-3.1.1)


file(GLOB_RECURSE CORE_SOURCE_FILES
          core/src/*.cpp
    	  core/include/*.hpp)

file(GLOB LINUX_SOURCE_FILES
          linux/src/utils/*.cpp)

include_directories(third core/include linux/include third/rapidjson third/cute_lib third/glfw-3.1.1/include)

add_compile_options(-std=c++11 -DBUILTIN_CUBEMAPS)

# ---- main ---- #
add_executable(arpigl-linux ${CORE_SOURCE_FILES} ${LINUX_SOURCE_FILES} linux/src/main.cpp)
target_link_libraries(arpigl-linux glfw ${GLFW_LIBRARIES} png16)


# ---- test ---- #
#add_executable(arpigl-linux-test ${CORE_SOURCE_FILES} ${LINUX_SOURCE_FILES} linux/src/GeoEngineTest.cpp)
#set_target_properties(arpigl-linux-test PROPERTIES COMPILE_FLAGS "-DNDEBUG")
#target_link_libraries(eventribe-linux-test glfw ${GLFW_LIBRARIES} png16)
