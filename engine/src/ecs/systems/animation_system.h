#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct animation_system : system {
    FUSE_INLINE void update(float) {
      for (auto& entity : _registry->view<animation_component>()) {
        auto& tr = _registry->get_component<transform_component>(entity).transform;
        auto& an = _registry->get_component<animation_component>(entity);

        // get animation from assets
        auto& anim = _assets->get<animation_asset>(an.animation)->instance;
        int index = (get_ticks_ms()/anim.speed) % anim.frames.size();
        auto& frame = _assets->get<texture_asset>(anim.frames[index])->instance;

        // destination viewport
        SDL_FRect dst_rect = { 
          tr.translate.x, tr.translate.y,
          frame.width * tr.scale.x, 
          frame.height * tr.scale.y 
        };

        // center of object
        const SDL_FPoint center = { 
          tr.translate.x + dst_rect.w/2.0f,
          tr.translate.y + dst_rect.h/2.0f
        };
        
        SDL_RenderCopyExF(_renderer, frame.data, NULL, &dst_rect, tr.rotation, &center, an.flip);
      }
    }
  };
}