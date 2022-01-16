#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct animation_system : system {
    FUSE_INLINE void update(float dt) {
      for (auto &entity : _registry->view<animation_component>()) {
        auto& tr = _registry->get_component<transform_component>(entity).transform;
        auto& id = _registry->get_component<animation_component>(entity).animation;

        // get animation from assets
        auto& anim = _assets->get<animation_asset>(id)->animation;
        int index = (SDL_GetTicks()/anim.speed) % anim.frame_count;
        auto& frame = _assets->get<texture_asset>(anim.frames[index])->texture;

        SDL_Rect dst_rect = { 
          tr.translate.x, tr.translate.y,
          frame.width * tr.scale.x, frame.height * tr.scale.y 
        };
        
        SDL_RenderCopyEx(this->_renderer, frame.data, NULL, &dst_rect, tr.rotation, NULL, anim.flip);
      }
    }
  };
}