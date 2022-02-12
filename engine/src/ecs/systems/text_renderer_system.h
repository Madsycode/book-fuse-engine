#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct text_renderer_system : system {
    FUSE_INLINE void update(float) {
      for (auto& e : view<text_component>()) {
        // get components
        auto& tr = e.get_component<transform_component>();
        auto& tx = e.get_component<text_component>();

        // get texture from asset registry
        auto& font = _assets->get<font_asset>(tx.font)->instance;

        // create text surface
        SDL_Surface *surface = NULL;
        surface = TTF_RenderText_Blended(font.data, tx.text.c_str(), tx.color);
        if (!surface) { 
          FUSE_ERROR("%s", TTF_GetError());
          continue;
        }

        // create texture from surface
        SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
          FUSE_ERROR("%s", SDL_GetError());
          continue;
        }

        // query texture size
        int width, height;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);

        // define target viewport
        SDL_FRect dst { 
          tr.translate.x, tr.translate.y,
          width * tr.scale.x, height * tr.scale.y
        };

        // render sprite        
        SDL_RenderCopyExF(_renderer, texture, NULL, &dst, tr.rotation, NULL, tx.flip);
        
        // destroy texture
        SDL_DestroyTexture(texture);
      }
    }
  };
}