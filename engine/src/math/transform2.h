#pragma once
#include "math/vec2.h"

namespace fuse {
  struct transform2 {
    FUSE_INLINE transform2(const transform2 &) = default;
    FUSE_INLINE transform2() = default;

    vec2f translate = vec2f(0.0f);
    vec2f scale = vec2f(1.0f);
    float rotation = 0.0f;
  };
}