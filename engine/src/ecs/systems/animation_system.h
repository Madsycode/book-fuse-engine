#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct animation_system : system {
    FUSE_INLINE void update(float) {
      for (auto& entity : _registry->view<animation_component>()) {
        auto& tr = _registry->get_component<transform_component>(entity).transform;
        auto& id = _registry->get_component<animation_component>(entity).animation;

        // get animation from assets
        auto& anim = _assets->get<animation_asset>(id)->animation;
        int index = (get_ticks_ms()/anim.speed) % anim.frame_count;
        auto& frame = _assets->get<texture_asset>(anim.frames[index])->texture;

        SDL_FRect dst_rect = { 
          tr.translate.x, tr.translate.y,
          frame.width * tr.scale.x, frame.height * tr.scale.y 
        };
        
        SDL_RenderCopyExF(_renderer, frame.data, NULL, &dst_rect, tr.rotation, NULL, anim.flip);
      }
    }
  };
}