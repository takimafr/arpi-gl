#include <utils/Log.hpp>
#include <stdexcept>
#include "utils/ExceptionUtils.hpp"

namespace dma {

    void ExceptionUtils::runtime(const std::string &tag, const std::string &error) {
        Log::error(tag, error);
        throw std::runtime_error(error);
    }
}

