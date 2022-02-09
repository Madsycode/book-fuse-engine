#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct script_system : system {
    FUSE_INLINE ~script_system() {
      for (auto& e: view<script_component>()) {
        auto& script = e.get_component<script_component>();
        FUSE_DELETE(script.instance);
      }
    }

    FUSE_INLINE void start() {
      for (auto& e : view<script_component>()) {
        auto& script = e.get_component<script_component>();
        script_props props(e, _registry, _assets);
        script.instance = script.instantiate(props);
        script.instance->on_start();
      }
    }

    FUSE_INLINE void update(float dt) {
      for (auto& e : view<script_component>()) {
        e.get_component<script_component>().instance->on_update(dt);
      }
    }
  };
}