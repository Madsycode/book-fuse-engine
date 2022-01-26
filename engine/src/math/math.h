#pragma once

#include "transform2.h"
#include "vec2.h"

namespace fuse::math {
  FUSE_INLINE int random(int min, int max) {
    static bool first = true;
    if (first) {
      srand(time(NULL)); // seeding for the first time only!
      first = false;
    }
    return min + rand() % ((max + 1) - min);
  }

  template <typename T>
  FUSE_INLINE void clamp(T& value, const T& min, const T& max) {
    value = std::max(min, std::min(value, max));
  }
}