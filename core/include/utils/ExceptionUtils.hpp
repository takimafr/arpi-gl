#ifndef ARPIGL_EXCEPTIONUTILS_HPP
#define ARPIGL_EXCEPTIONUTILS_HPP

#include <string>

namespace dma {
    class ExceptionUtils {
    public:
        static void runtime(const std::string& tag, const std::string& error);
    };
}


#endif //ARPIGL_EXCEPTIONUTILS_HPP
