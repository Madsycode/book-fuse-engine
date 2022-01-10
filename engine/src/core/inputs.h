#pragma once
#include "math/vec2.h"
#include "core/dispatcher.h"

namespace fuse::inputs {
    FUSE_API vec2i mouse_wheel();
    FUSE_API vec2i mouse_position();

    FUSE_API bool is_pressed(int key);
    FUSE_API bool is_down(int button);

    FUSE_API void process_sdl_inputs();
    FUSE_API dispatcher* get_dispatcher();
}