#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct script_system : system {
    FUSE_INLINE ~script_system() {
      for (auto& entity : _registry->view<script_component>()) {
        auto& script = _registry->get_component<script_component>(entity);
        FUSE_DELETE(script.instance);
      }
    }

    FUSE_INLINE void start() {
      for (auto& e : _registry->view<script_component>()) {
        auto& script = _registry->get_component<script_component>(e);
        script_props props(e, _registry, _assets);
        script.instance = script.allocate(props);
        script.instance->on_start();
      }
    }

    FUSE_INLINE void update(float dt) {
      for (auto& entity : _registry->view<script_component>()) {
        _registry->get_component<script_component>(entity).instance->on_update(dt);
      }
    }
  };
}