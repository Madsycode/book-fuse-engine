#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct sprite_renderer_system : system {
    FUSE_INLINE void update(float) {
      for (auto &entity : _registry->view<sprite_component>()) {
        auto& tr = _registry->get_component<transform_component>(entity).transform;
        auto& sp = _registry->get_component<sprite_component>(entity);

        // get texture from asset_registry
        auto& sprite = _assets->get<texture_asset>(sp.sprite)->texture;

        SDL_FRect dst_rect{
          tr.translate.x, tr.translate.y,
          sprite.width * tr.scale.x, 
          sprite.height * tr.scale.y
        };

        // center of object
        const SDL_FPoint center = { 
          tr.translate.x + dst_rect.w/2.0f,
          tr.translate.y + dst_rect.h/2.0f
        };

        // render sprite        
        SDL_RenderCopyExF(_renderer, sprite.data, NULL, &dst_rect, tr.rotation, &center, sp.flip);
      }
    }
  };
}