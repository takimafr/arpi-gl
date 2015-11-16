/*
Copyright © 2015 by eBusiness Information
All rights reserved. This source code or any portion thereof
may not be reproduced or used in any manner whatsoever
without the express written permission of eBusiness Information.
*/


#include <unistd.h>

#include "GeoEngineTest.h"

using namespace dma;
using namespace dma::geo;


const std::string TestResource::resFolder = "res-test";
const std::string TestResource::Mesh::cube = ResourceManager::ResourceIds::Mesh::CUBE;
const std::string TestResource::Material::cube = "cube";
const std::string TestResource::Scene::inno= "inno";
const std::string TestResource::Scene::invalid= "invalid";


void testLog() {
    Log::trace(LOG_TAG, "Hello %s %c", "world", '!');
    Log::debug(LOG_TAG, "Hello %s %c", "world", '!');
    Log::info(LOG_TAG, "Hello %s %c", "world", '!');
    Log::warn(LOG_TAG, "Hello %s %c", "world", '!');
    Log::error(LOG_TAG, "Hello %s %c", "world", '!');
}

void testObjReader() {

    Log::info(LOG_TAG, "===== testing %s =====", "objReader");

    ObjReader objReader(TestResource::resFolder + "/mesh/cube.obj");

    ASSERTM("obj reader failed gotoPositions()", objReader.gotoPositions() == STATUS_OK);
    ASSERTM("obj reader failed objReader()", objReader.gotoNormals() == STATUS_OK);
    ASSERTM("obj reader failed gotoFaces()", objReader.gotoFaces() == STATUS_OK);

    objReader.gotoPositions();
    glm::vec3 position;
    while(objReader.nextPosition(position)) {
        Log::debug(LOG_TAG, "VERTEX: %f %f %f", position.x, position.y, position.z);
    }

    objReader.gotoNormals();
    glm::vec3 normal;
    while(objReader.nextNormal(normal)) {
        Log::debug(LOG_TAG, "NORMAL: %f %f %f", normal.x, normal.y, normal.z);
    }



    objReader.gotoFaces();
    U16 face[3][3];
    while(objReader.nextFace(face)) {
        Log::debug(LOG_TAG, "FACE: %d/%d/%d %d/%d/%d %d/%d/%d",
                face[0][0], face[0][1], face[0][2],
                face[1][0], face[1][1], face[1][2],
                face[2][0], face[2][1], face[2][2]);

    }
}


void testMaterialReader() {
    Log::trace(LOG_TAG, "Starting testMaterialReader");
    MaterialReader materialReader("res/material/test.json");
    materialReader.parse();
    std::string shader = materialReader.getShader();
    std::string diffuseMap = materialReader.getDiffuseMap();
    Log::trace(LOG_TAG, "shader = %s | diffuseMap = %s", shader.c_str(), diffuseMap.c_str());
    Log::trace(LOG_TAG, "testMaterialReader done");
}



void testEngineLifecycle() {

    Log::trace(LOG_TAG, "===== testing %s =====", "EngineLifecycle");

    for(int i=0; i<2; ++i) {
        onInit();
        TimeoutCallback callback;
        Log::info(LOG_TAG, "-----------------");
        Log::info(LOG_TAG, "Blue window popped up? [Y]/n");
        Log::info(LOG_TAG, "-----------------");
        mainLoop(&callback);
        onDestroy();
    }
}



void testRotatingCubes() {
    std::string testName = "RotatingCubes";
    struct RotatingCubeCallback : TimeoutCallback {
        Poi* mCenter;
        Poi* mLeft;
        Poi* mRight;
        Poi* mFront;
        Poi* mBack;
        GeoEngine* mEngine;

        RotatingCubeCallback(GeoEngine* engine, Poi* center, Poi* left, Poi* right, Poi* front, Poi* back) :
            TimeoutCallback(DEFAULT) {
            mCenter = center;
            mLeft = left;
            mRight = right;
            mFront = front;
            mBack = back;
            mEngine = engine;
        }

        virtual void operator()() {
            TimeoutCallback::operator()();
            mLeft->rotate(.1, 0, 0, 1);
            mRight->yaw(-.1);
            mFront->pitch(.1);
            mBack->pitch(-.1);
            mCenter->roll(.1);
        }
    };


    for(bool destroyPoi : {false, true}) {

        Log::info(LOG_TAG, "===== testing %s =====", testName.c_str());


        onInit();
        PoiFactory& factory = mGeoEngine->getPoiFactory();
        const double dist = 0.0001;
        Status ires;
        const std::string& meshSid = TestResource::Mesh::cube;
        const std::string& matSid = TestResource::Material::cube;

//        Poi& centerCube = factory.createPoi(meshSid, matSid);
//        Poi& leftCube = factory.createPoi(meshSid, matSid, &ires, 0, -dist, 0); ASSERT(ires == STATUS_OK);
//        Poi& rightCube = factory.createPoi(meshSid, matSid, &ires, 0, dist, 0); ASSERT(ires == STATUS_OK);
//        Poi& frontCube = factory.createPoi(meshSid, matSid, &ires, -dist, 0); ASSERT(ires == STATUS_OK);
//        Poi& backCube = factory.createPoi(meshSid, matSid, &ires, dist, 0); ASSERT(ires == STATUS_OK);
        PoiParams centerCube = PoiParams("centerCube", meshSid, "cube_texture", Color(0.2f, 0.2f, 0.3f));
        PoiParams leftCube = PoiParams("leftCube", meshSid, "cube_texture", Color(0.2f, 0.2f, 0.3f), 0.0, -dist, 0.0);
        PoiParams rightCube = PoiParams("rightCube", meshSid, "cube_texture", Color(0.2f, 0.2f, 0.3f), 0.0, dist, 0.0);
        PoiParams frontCube = PoiParams("frontCube", meshSid, "cube_texture", Color(0.2f, 0.2f, 0.3f), -dist, 0.0, 0.0);
        PoiParams backCube = PoiParams("backCube", meshSid, "cube_texture", Color(0.2f, 0.2f, 0.3f), dist, 0.0, 0.0);

        mGeoEngine->addPoi(centerCube);
        mGeoEngine->addPoi(leftCube);
        mGeoEngine->addPoi(rightCube);
        mGeoEngine->addPoi(frontCube);
        mGeoEngine->addPoi(backCube);

        RotatingCubeCallback callback(mGeoEngine, &centerCube, &leftCube, &rightCube, &frontCube, &backCube);

        Log::info(LOG_TAG, "-----------------");
        Log::info(LOG_TAG, "View from top of the scene.");
        Log::info(LOG_TAG, "Center cube should roll CCW");
        Log::info(LOG_TAG, "Left cube should yaw CCW, right one way CW.");
        Log::info(LOG_TAG, "5 cube? rotation ok? [Y]/n");
        Log::info(LOG_TAG, "-----------------");

        mainLoop(&callback);
        if(destroyPoi) {

            mGeoEngine->removePoi(leftCube.sid);
            mGeoEngine->removePoi(rightCube.sid);
            mGeoEngine->removePoi(frontCube.sid);
            mGeoEngine->removePoi(backCube.sid);

            factory.deletePoi(leftCube.sid);
            factory.deletePoi(rightCube.sid);
            factory.deletePoi(frontCube.sid);
            factory.deletePoi(backCube.sid);
        }

        onDestroy();
        Log::info(LOG_TAG, "===== testing %s OK =====", testName.c_str());
    }
}

void testSkybox() {
    std::string testName = "testSkybox";
    struct SkyboxCallback : TimeoutCallback {
        GeoEngine * mEngine;
        int mCurrentFrame;
        const float YAW_DELTA = 0.5;

        SkyboxCallback(GeoEngine * engine) : TimeoutCallback(DEFAULT) {
            mCurrentFrame = 0;
            mEngine = engine;
        }

        virtual void operator()() {
            TimeoutCallback::operator()();
            mCurrentFrame++;
            mFlyThroughCamera->yaw(YAW_DELTA);
        }
    };

    Log::info(LOG_TAG, "===== testing %s =====", testName.c_str());
    onInit();
    mGeoEngine->setSkyBoxEnabled(true);

    mFlyThroughCamera->pitch(90);
    SkyboxCallback callback(mGeoEngine);

    Log::info(LOG_TAG, "-----------------");
    Log::info(LOG_TAG, "skybox rotating CW ? [Y]/n");
    Log::info(LOG_TAG, "-----------------");
    mainLoop(&callback);
    onDestroy();
    Log::info(LOG_TAG, "===== testing %s OK =====", testName.c_str());

}


void testSpawningCubes() {

    std::string testName = "SpawningCubes";
    struct SpawnCubeCallback : TimeoutCallback {
        PoiParams* mPois[4];
        GeoEngine * mEngine;
        const int SPAWN_PERIOD = 50;    // frames
        int mCurrentFrame;
        int mCurrentVisible;

        SpawnCubeCallback(GeoEngine * engine, Poi* pois[4]) : TimeoutCallback(DEFAULT) {
            mCurrentFrame = 0;
            mCurrentVisible = 0;
            mEngine = engine;
            for(int i=0; i<4; ++i) {
                mPois[i] = pois[i];
            }
            mEngine->addPoi(*mPois[mCurrentVisible]);

        }

        virtual void operator()() {
            TimeoutCallback::operator()();
            mCurrentFrame++;

            if(mCurrentFrame % SPAWN_PERIOD == 0) {
                mEngine->removePoi(*mPois[mCurrentVisible]->sid);
                mCurrentVisible += 1;
                (mCurrentVisible)%=4;
                mEngine->addPoi(*mPois[mCurrentVisible]);
            }
        }
    };


    Log::info(LOG_TAG, "===== testing %s =====", testName.c_str());

    onInit();
    const double dist = 0.0001;
    PoiFactory& factory = mGeoEngine->getPoiFactory();
    Status ires;

    const std::string& meshSid = TestResource::Mesh::cube;
    const std::string& matSid = TestResource::Material::cube;

    Poi& leftCube = factory.createPoi(meshSid, matSid, &ires, 0, -dist, 0); ASSERT(ires == STATUS_OK);
    Poi& rightCube = factory.createPoi(meshSid, matSid, &ires, 0, dist, 0); ASSERT(ires == STATUS_OK);
    Poi& frontCube = factory.createPoi(meshSid, matSid, &ires, -dist, 0); ASSERT(ires == STATUS_OK);
    Poi& backCube = factory.createPoi(meshSid, matSid, &ires, dist, 0); ASSERT(ires == STATUS_OK);

    Poi* pois[4] = {&leftCube, &rightCube, &frontCube, &backCube};
    SpawnCubeCallback callback(mGeoEngine, pois);

    Log::info(LOG_TAG, "-----------------");
    Log::info(LOG_TAG, "4 cube spawning? [Y]/n");
    Log::info(LOG_TAG, "-----------------");
    mainLoop(&callback);
    onDestroy();
    Log::info(LOG_TAG, "===== testing %s OK =====", testName.c_str());

}

void testPositionningCube() {

    std::string testName = "PositionningCube";
    struct PositionCubeCallback : TimeoutCallback {
        Poi* mPoi;
        GeoEngine * mEngine;
        const int POSITION_PERIOD = 10;    // frames
        const double POSITION_DELTA = 0.00001;    // lon
        int mCurrentFrame;


        PositionCubeCallback(GeoEngine * engine, Poi* poi) : TimeoutCallback(DEFAULT) {
            mCurrentFrame = 0;
            mEngine = engine;
            mPoi = poi;
            mEngine->addPoi(*mPoi);
        }

        virtual void operator()() {
            TimeoutCallback::operator()();
            mCurrentFrame++;


            if(mCurrentFrame % POSITION_PERIOD == 0) {
                double pos[3];
                mPoi->getPosition(pos);
                pos[1] += POSITION_DELTA;
                mPoi->setPosition(pos);
            }
        }
    };


    Log::info(LOG_TAG, "===== testing %s =====", testName.c_str());

    onInit();
    PoiFactory& factory = mGeoEngine->getPoiFactory();
    const double dist = 0.0001;

    const std::string& meshSid = TestResource::Mesh::cube;
    const std::string& matSid = TestResource::Material::cube;
    Poi& cube = factory.createPoi(meshSid, matSid, 0, -dist, 0);

    PositionCubeCallback callback(mGeoEngine, &cube);

    Log::info(LOG_TAG, "-----------------");
    Log::info(LOG_TAG, "1 cube moving left to right ? [Y]/n");
    Log::info(LOG_TAG, "-----------------");
    mainLoop(&callback);
    onDestroy();
    Log::info(LOG_TAG, "===== testing %s OK =====", testName.c_str());

}

void testLoadScene() {
    std::string testName = "testLoadScene";
    Log::info(LOG_TAG, "===== testing %s =====", testName.c_str());

    onInit();

    mGeoEngine->loadGeoScene(TestResource::Scene::inno);
    mGeoCamera = &mGeoEngine->getCamera();
    const double* pos = mGeoCamera->getPosition();
    mGeoCamera->setPosition(pos[0], pos[1], 70);

    Log::info(LOG_TAG, "-----------------");
    Log::info(LOG_TAG, " scene with loaded with valid map ? [Y]/n");
    Log::info(LOG_TAG, "-----------------");
    mainLoop();
    onDestroy();
    Log::info(LOG_TAG, "===== testing %s OK =====", testName.c_str());

}

void testLoadNullScene() {
    std::string testName = "testLoadNullScene";
    Log::info(LOG_TAG, "===== testing %s =====", testName.c_str());

    onInit();

    mGeoEngine->loadGeoScene(TestResource::Scene::invalid);

    Log::info(LOG_TAG, "-----------------");
    Log::info(LOG_TAG, " scene with loaded with invalid map ? [Y]/n");
    Log::info(LOG_TAG, "-----------------");
    mainLoop();

    onDestroy();
    Log::info(LOG_TAG, "===== testing %s OK =====", testName.c_str());

}


void testFallbackMesh() {
    std::string testName = "testInvalidPoi";
    Log::info(LOG_TAG, "===== testing %s =====", testName.c_str());

    onInit();

    mGeoEngine->setOrigin(0.0, 0.0);
    mGeoCamera->setPosition(0, 0);
    Status result;
    Poi& poi = mGeoEngine->getPoiFactory().createPoi("IDon'tExist", "IDon'tExist", "IDon'tExist", &result);
    ASSERTM("loading this poi should return an error.", result != STATUS_OK);
    mGeoEngine->addPoi(poi);
    Log::info(LOG_TAG, "-----------------");
    Log::info(LOG_TAG, " fallback mesh visible? [Y]/n");
    Log::info(LOG_TAG, "-----------------");
    mainLoop();
    onDestroy();
    Log::info(LOG_TAG, "===== testing %s OK =====", testName.c_str());

}

void runAllTests(int argc, char const *argv[]) {

#ifdef TEST_RESOURCES
    mCuteSuite.push_back(CUTE(testLog));
    s.push_back(CUTE(testMaterialReader));
    mCuteSuite.push_back(CUTE(testObjReader));
#endif

    mCuteSuite.push_back(CUTE(testEngineLifecycle));
    mCuteSuite.push_back(CUTE(testSkybox));
    mCuteSuite.push_back(CUTE(testRotatingCubes));
    mCuteSuite.push_back(CUTE(testSpawningCubes));
    mCuteSuite.push_back(CUTE(testPositionningCube));
    mCuteSuite.push_back(CUTE(testLoadScene));

#ifdef NDEBUG
    mCuteSuite.push_back(CUTE(testLoadNullScene));
    mCuteSuite.push_back(CUTE(testFallbackMesh));
#endif

    cute::xml_file_opener xmlfile(argc, argv);
    cute::xml_listener<cute::ide_listener<> > lis(xmlfile.out);
    cute::makeRunner(lis, argc, argv)(mCuteSuite, "AllTests");
}

int main(int argc, const char **argv) {
    Log::info(LOG_TAG, "Starting tests...");
    Log::info(LOG_TAG, "==============================");

    runAllTests(argc, argv);

    Log::info(LOG_TAG, "==============================");
    Log::info(LOG_TAG, "tests finished");

    return 0;
}



