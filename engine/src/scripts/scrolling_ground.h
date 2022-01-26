#pragma once

#include "script_instance.h"

namespace fuse {
  struct scrolling_ground : script_instance {
    FUSE_INLINE void on_start() {
      width = get_asset<texture_asset>("ground")->texture.width;
    }

    FUSE_INLINE void on_update(float dt) {    
      auto& tr = get_component<ecs::transform_component>();
      if(tr.transform.translate.x + width < SCREEN_WIDTH) {
        tr.transform.translate.x = 0.0;
      }
    }

  private:
    float width;
  };
}