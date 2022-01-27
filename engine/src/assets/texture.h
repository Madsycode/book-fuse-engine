#pragma once
#include "asset_instance.h"

namespace fuse {
  struct texture_instance {    
    SDL_Texture *data = NULL;
    std::string filename;
    int height = 0;
    int width = 0;
  };

  struct texture_asset : asset_instance {
    FUSE_INLINE texture_asset(const std::string& name, const texture_instance& data) {
      this->texture = data;
      this->name = name;
    }

    FUSE_INLINE texture_asset() = default;

    FUSE_INLINE ~texture_asset() { 
      SDL_DestroyTexture(texture.data);
    }

    texture_instance texture;
  };
}