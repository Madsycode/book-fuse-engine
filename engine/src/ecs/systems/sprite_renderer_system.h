#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct sprite_renderer_system : system {
    FUSE_INLINE void update(float) {
      for (auto &entity : _registry->view<sprite_component>()) {
        auto& tr = _registry->get_component<transform_component>(entity).transform;
        auto& id = _registry->get_component<sprite_component>(entity).sprite;

        // get texture from asset_registry
        auto& sprite = _assets->get<texture_asset>(id)->texture;

        SDL_FRect dst_rect{
          tr.translate.x, tr.translate.y,
          sprite.width * tr.scale.x, sprite.height * tr.scale.y
        };

        // render sprite        
        SDL_RenderCopyExF(_renderer, sprite.data, NULL, &dst_rect, tr.rotation, NULL, sprite.flip);
      }
    }
  };
}