#include "animation/GeoPathAnimation.hpp"

namespace dma {

    float getTime(glm::vec3& a, glm::vec3& b, float speed) {
        float d = glm::length<float>(b - a);
        return d / speed;
    }

    GeoPathAnimation::GeoPathAnimation(TransformComponent &transformComponent,
                                       std::vector<LatLngAlt> &path,
                                       float speed, bool loop,
                                       GeoSceneManager& geoSceneManager) :
            Animation(transformComponent, 0.0f, loop),
            mPath(path),
            mGeoSceneManager(geoSceneManager)
    {
        int n = (int) mPath.size();
        for (int i = 0; i < n-1; ++i) {
            glm::vec3 a = mGeoSceneManager.mapPosition(mPath[i]);
            glm::vec3 b = mGeoSceneManager.mapPosition(mPath[i+1]);
            float t = getTime(a, b, speed);
            mDurations.push_back(t);
            mDuration += t;
        }

        if (loop) {
            glm::vec3 a = mGeoSceneManager.mapPosition(mPath[n-1]);
            glm::vec3 b = mGeoSceneManager.mapPosition(mPath[0]);
            float t = getTime(a, b, speed);
            mDurations.push_back(t);
            mDuration += t;
        }
    }

    void GeoPathAnimation::update(float dt) {
        mCurrentTime += dt;
        if (mCurrentTime > mDuration) {
            mCurrentTime = mLoop ? glm::mod<float>(mCurrentTime, mDuration) : mDuration;
        }

        float t = 0.0f;
        int i = 0;
        while (mCurrentTime > t) {
            t += mDurations[i++];
        }

        float y = mTransformComponent->getPosition().y;
        mGeoSceneManager.placeCamera(mPath[i % mPath.size()]);

        float x = (mCurrentTime - (t - mDurations[i-1])) / (mDurations[i-1]);
        glm::vec3 from = mGeoSceneManager.mapPosition(mPath[i-1]);
        glm::vec3 to = mGeoSceneManager.mapPosition(mPath[i % mPath.size()]);

        from.y = y;
        to.y = y;

        mTransformComponent->setPosition(glm::mix(from, to, x));
    }

}

