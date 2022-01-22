#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct rigidbody_system : system {
    FUSE_INLINE void update(float dt) {
      for (auto& entity : _registry->view<rigidbody_component>()) {
        auto& bd = _registry->get_component<rigidbody_component>(entity).body;
        if (bd.disabled) { continue; }

        // update rigid body
        auto Fg = vec2f(0.0f, gravity * bd.gravity_scale);
        bd.velocity += (bd.impulse + bd.force + Fg + bd.friction) * dt;        
        _registry->get_component<transform_component>(entity).transform.translate += (bd.velocity * dt);
        // reset impulse
        bd.apply_impulse(0.0f);
      }
    }

  private:
    float gravity = 9800.0f;
  };
}