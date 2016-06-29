#include <utils/Log.hpp>
#include "animation/PathAnimation.hpp"

namespace dma {

    float time(glm::vec3& a, glm::vec3& b, float speed) {
        float d = glm::length<float>(b - a);
        return d / speed;
    }

    PathAnimation::PathAnimation(dma::TransformComponent &transformComponent,
                                 std::vector<glm::vec3>& path,
                                 float speed,
                                 bool loop) :
            Animation(transformComponent, 0.0f, loop),
            mPath(path),
            mSpeed(speed)
    {
        int n = (int) mPath.size();
        for (int i = 0; i < n-1; ++i) {
            float t = time(path[i], path[i+1], speed);
            mDurations.push_back(t);
            mDuration += t;
        }

        if (loop) {
            float t = time(path[n-1], path[0], speed);
            mDurations.push_back(t);
            mDuration += t;
        }
    }

    void PathAnimation::update(float dt) {
        mCurrentTime += dt;
        if (mCurrentTime > mDuration) {
            mCurrentTime = mLoop ? glm::mod<float>(mCurrentTime, mDuration) : mDuration;
        }

        float t = 0.0f;
        int i = 0;
        while (mCurrentTime > t) {
            t += mDurations[i++];
        }

        float x = (mCurrentTime - (t - mDurations[i-1])) / (mDurations[i-1]);
        glm::vec3 from = mPath[i-1];
        glm::vec3 to = mPath[i % mPath.size()];

        mTransformComponent->setPosition(glm::mix(from, to, x));
    }
}

