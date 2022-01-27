#pragma once

#include "script_instance.h"

namespace fuse {
  struct scrolling_ground : script_instance {
    FUSE_INLINE void on_start() {
      width = get_asset<texture_asset>("ground")->instance.width;
    }

    FUSE_INLINE void on_update(float dt) {    
      auto& tr = get_component<ecs::transform_component>();
      if(tr.transform.translate.x + width < inputs::display_size().x) {
        tr.transform.translate.x = 0.0;
      }
    }

  private:
    float width;
  };
}