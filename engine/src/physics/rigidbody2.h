#pragma once

#include "math/vec2.h"

namespace fuse {
  struct rigidbody2 {
    FUSE_INLINE rigidbody2() = default;

    // force
    FUSE_INLINE void apply_force_x(float x) { force.x = x; }
    FUSE_INLINE void apply_force_y(float y) { force.y = y; }
    FUSE_INLINE void apply_force(const vec2f& f) { force = f; }

    // friction
    FUSE_INLINE void apply_friction_x(float x) { friction.x = x; }
    FUSE_INLINE void apply_friction_y(float y) { friction.y = y; }
    FUSE_INLINE void apply_friction(const vec2f& f) { friction = f; }

    // impulse
    FUSE_INLINE void apply_impulse_x(float x) { impulse.x = x; }
    FUSE_INLINE void apply_impulse_y(float y) { impulse.y = y; }
    FUSE_INLINE void apply_impulse(const vec2f& i) { impulse = i; }

    float gravity_scale = 1.0f;
    bool disabled = false;
    vec2f velocity;
    vec2f friction;
    vec2f impulse;
    vec2f force;
  };
}