#ifndef ARPIGL_COMMAND_HPP
#define ARPIGL_COMMAND_HPP

#include <common/Types.hpp>

namespace dma {
    class Command {
    public:
        static const U32 MOVE_TO    = 1;
        static const U32 LINE_TO    = 2;
        static const U32 CLOSE_PATH = 7;
    };
}


#endif //ARPIGL_COMMAND_HPP
