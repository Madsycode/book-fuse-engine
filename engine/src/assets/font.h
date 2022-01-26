#pragma once
#include "asset_instance.h"

namespace fuse {
  struct font_asset : asset_instance {
    FUSE_INLINE font_asset(const std::string& path, const std::string& name, TTF_Font* font) {
      this->id = generate_uuid();
      this->font = font;
      this->name = name;
      this->path = path;
    }

    FUSE_INLINE font_asset() = default;

    FUSE_INLINE ~font_asset() {
      TTF_CloseFont(font);
    }

    TTF_Font* font = NULL;
  };
}