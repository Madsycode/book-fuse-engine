#pragma once
#include "asset_instance.h"

namespace fuse {
  struct texture_instance {
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Texture *data = NULL;
    int height = 0;
    int width = 0;
  };

  struct texture_asset : asset_instance {
    FUSE_INLINE texture_asset(const std::string& path, const std::string& name, const texture_instance& data) {
      this->id = generate_uuid();
      this->texture = data;
      this->name = name;
      this->path = path;
    }

    FUSE_INLINE texture_asset() = default;

    FUSE_INLINE ~texture_asset() { 
      SDL_DestroyTexture(texture.data);
    }

    texture_instance texture;
  };
}