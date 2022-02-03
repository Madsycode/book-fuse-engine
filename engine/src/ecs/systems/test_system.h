#include "ecs/system.h"
#include "ecs/components/common.h"

namespace fuse::ecs {
  struct test_system : system {
    void update(float) {
      for (auto& e : view<transform_component>()) {
        auto& t = e.get_component<transform_component>();        
        FUSE_INFO("id:[%d], x:%f", e.id(), t.translate.x++);
      }
    }
  };
}