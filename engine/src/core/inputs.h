#pragma once
#include "math/vec2.h"
#include "core/dispatcher.h"

namespace fuse::inputs {
  struct mouse_state {
    std::bitset<7> buttons = {0};
    vec2f offset;
    vec2f wheel;
  };

  FUSE_API const vec2f& mouse_wheel();
  FUSE_API const vec2f& mouse_offset();
  FUSE_API const vec2f& display_size();

  FUSE_API bool is_button(int);
  FUSE_API bool is_key(int);

  FUSE_API dispatcher* get_disp();
  void initialize(SDL_Window*);
  void dispatch_events();
}