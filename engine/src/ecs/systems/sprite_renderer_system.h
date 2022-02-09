#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct sprite_renderer_system : system {
    FUSE_INLINE void update(float) {
      for (auto& e : view<sprite_component>()) {
        // get components
        auto& tr = e.get_component<transform_component>();
        auto& sp = e.get_component<sprite_component>();

        // get texture from asset registry
        auto& sprite = _assets->get<texture_asset>(sp.sprite)->instance;

        // define target viewport
        SDL_FRect dst{ 
          tr.translate.x, tr.translate.y,
          sprite.width * tr.scale.x, sprite.height * tr.scale.y
        };

        // render sprite        
        SDL_RenderCopyExF(_renderer, sprite.data, NULL, &dst, tr.rotation, NULL, sp.flip);
      }
    }
  };
}