#pragma once
#include "ecs/registry.h"
#include "system_instance.h"
#include "ecs/components/common.h"

namespace fuse::ecs {
  struct test_system : system_instance {
    FUSE_INLINE test_system() = default;

    FUSE_INLINE void update(float dt) {
      for(auto& entity : _registry->view<transform_component>()) {
        auto& t = _registry->get_component<transform_component>(entity).transform;
        FUSE_INFO("id:[%d], x:%f", entity, t.translate.x);
        t.translate.x++;
      }
    }
  };
}