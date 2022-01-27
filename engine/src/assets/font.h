#pragma once
#include "asset.h"

namespace fuse {
  struct font {
    TTF_Font* data = NULL;
    std::string filename;
    int size = 20;
  };

  struct font_asset : asset {
    FUSE_INLINE font_asset(const std::string& name, const font& font) {
      this->font = font;
      this->name = name;
    }

    FUSE_INLINE font_asset() = default;

    FUSE_INLINE ~font_asset() {
      TTF_CloseFont(font.data);
    }

    font font;
  };
}