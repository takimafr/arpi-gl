#include <resource/ResourceManagerHandler.hpp>
#include <utils/Log.hpp>

constexpr auto TAG = "ResourceManagerHandler";

namespace dma {

    ResourceManagerHandler::~ResourceManagerHandler() {

    }

    template <class T>
    void ResourceManagerHandler::init() {
        mFallback = std::make_shared<T>();
        load(mFallback, FALLBACK_SID);
    }

    template <class T>
    std::shared_ptr<T> ResourceManagerHandler::acquire(const std::string& sid) {
        if (sid == FALLBACK_SID) {
            return mFallback;
        }
        if (mResources.find(sid) == mResources.end()) {
            std::shared_ptr<T> resource = std::make_shared<T>();
            if (load(resource, sid) != STATUS_OK) {
                Log::warn(TAG, "Resource %s doesn't exist, returning fallback instead", sid.c_str());
                return mFallback;
            }
            mResources[sid] = resource;
        }
        return mResources[sid];
    }

    void ResourceManagerHandler::unload() {
        Log::trace(TAG, "Unloading...");
        if (mFallback != nullptr) {
            mFallback = nullptr; //release reference count
        }
        mResources.clear();
        Log::trace(TAG, "Unloaded done...");
    }

    void ResourceManagerHandler::prune() {
        auto it = mResources.begin();
        while (it != mResources.end()) {
            if (it->second.unique()) {
                it = mResources.erase(it);
            } else {
                ++it;
            }
        }
    }
}