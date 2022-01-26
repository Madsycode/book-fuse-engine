#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct rigidbody_system : system {
    FUSE_INLINE void update(float dt) {
      for (auto& e : _registry->view<rigidbody_component>()) {
        auto& bd = _registry->get_component<rigidbody_component>(e);
        if(bd.disabled) { continue; }

        auto& tr = _registry->get_component<transform_component>(e);
        vec2f f_g = vec2f(0.0f, bd.body.gravity_scale * 9.8f);
        bd.body.velocity += (bd.body.force + f_g) * dt;
        tr.transform.translate += bd.body.velocity * dt;
      }
    }
  };
}