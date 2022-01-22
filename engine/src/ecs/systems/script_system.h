#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct script_system : system {
    FUSE_INLINE void start() {
      for (auto& e : _registry->view<script_component>()) {
        auto& script = _registry->get_component<script_component>(e);
        script.instantiate(ecs::entity(e, _registry), _assets);
        script.script->on_start();
      }
    }

    FUSE_INLINE void update(float dt) {
      for (auto& entity : _registry->view<script_component>()) {
        _registry->get_component<script_component>(entity).script->on_update(dt);
      }
    }
  };
}