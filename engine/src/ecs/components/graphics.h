#pragma once
#include "assets/asset_instance.h"

namespace fuse::ecs {
  struct sprite_component {
    FUSE_INLINE sprite_component(const sprite_component&) = default;
    FUSE_INLINE sprite_component(asset_id id) : sprite(id) {}
    FUSE_INLINE sprite_component() = default;
    asset_id sprite = INVALID_ID;
  };

  struct text_component {
    FUSE_INLINE text_component(asset_id font, const char* text): font(font), text(text) {}
    FUSE_INLINE text_component(const text_component &) = default;
    FUSE_INLINE text_component() = default;

    SDL_Color color = {0, 0, 0, 255};
    asset_id font = INVALID_ID;
    std::string text;
  };

  struct animation_component {
    FUSE_INLINE animation_component(const animation_component &) = default;
    FUSE_INLINE animation_component(asset_id id) : animation(id) {}
    FUSE_INLINE animation_component() = default;
    asset_id animation = INVALID_ID;
  };

  struct tilemap_component {
    FUSE_INLINE tilemap_component(const tilemap_component &) = default;
    FUSE_INLINE tilemap_component(asset_id id) : tilemap(id) {}
    FUSE_INLINE tilemap_component() = default;
    asset_id tilemap = INVALID_ID;
  };

  struct tile_component {
    FUSE_INLINE tile_component(const tile_component&) = default;
    FUSE_INLINE tile_component() = default;

    asset_id tilemap = INVALID_ID;
    asset_id tileset = INVALID_ID;
    int row, col = 0;
    int x, y = 0;
  };
}