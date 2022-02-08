#pragma once
#include "math/vec2.h"

namespace fuse {
  struct rigidbody2 {
    FUSE_INLINE void set_force(const vec2f& f) { force = f; }
    FUSE_INLINE void set_force_x(float x) { force.x = x; }
    FUSE_INLINE void set_force_y(float y) { force.y = y; }

    float gravity_scale = 0.0f;
    vec2f velocity;
    vec2f force;
  };
}