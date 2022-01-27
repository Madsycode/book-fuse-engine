#pragma once
#include "core/event.h"

namespace fuse {
  struct quit_event : event_instance {
    FUSE_INLINE quit_event() = default;
  };

  struct resized_event : event_instance {
    FUSE_INLINE resized_event(int w, int h): width(w), height(h){}
    int width, height;
  };

  struct keydown_event : event_instance {
    FUSE_INLINE keydown_event(int key) : key(key) {}
    int key;
  };

  struct keyup_event : event_instance {
    FUSE_INLINE keyup_event(int key) : key(key) {}
    int key;
  };

  struct keyrepeat_event : event_instance {
    FUSE_INLINE keyrepeat_event(int key) : key(key) {}
    int key;
  };

  struct mousedown_event : event_instance {
    FUSE_INLINE mousedown_event(int btn):button(btn) {}
    int button;
  };

  struct mouseup_event : event_instance {
    FUSE_INLINE mouseup_event(int btn) : button(btn) {}
    int button;
  };

  struct mousemotion_event : event_instance {
    FUSE_INLINE mousemotion_event() = default;
  };

  struct mousewheel_event : event_instance {
    FUSE_INLINE mousewheel_event() = default;
  };
}