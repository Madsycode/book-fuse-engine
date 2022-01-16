#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct text_renderer_system : system {
    FUSE_INLINE void update(float dt) {
      for (auto& entity : _registry->view<text_component>()) {
        auto& tr = _registry->get_component<transform_component>(entity).transform;
        auto& tx = _registry->get_component<text_component>(entity);

        // get font from asset registry
        auto& font = _assets->get<font_asset>(tx.font)->font;
        
        // create text surface
        SDL_Surface *surface = NULL;
        surface = TTF_RenderText_Blended(font.data, tx.text.c_str(), tx.color);
        if (!surface) {
          FUSE_ERROR("%s", TTF_GetError());
          continue;
        }

        // create texture from surface
        SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
          FUSE_ERROR("%s", SDL_GetError());
          continue;
        }

        // get texture size
        int width, height;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        const SDL_Rect dst_rect = { 
          tr.translate.x, tr.translate.y, 
          width * tr.scale.x, height * tr.scale.y
        };

        // render texture
        SDL_RenderCopyEx(_renderer, texture, NULL, &dst_rect, tr.rotation, NULL, SDL_FLIP_NONE);
        SDL_DestroyTexture(texture);
      }
    }
  };
}