#pragma once
#include "asset_instance.h"

namespace fuse {
  struct font_instance {
    TTF_Font* data = NULL;
    std::string filename;
    int size = 20;
  };

  struct font_asset : asset_instance {
    FUSE_INLINE font_asset(const std::string& name, const font_instance& font) {
      this->font = font;
      this->name = name;
    }

    FUSE_INLINE font_asset() = default;

    FUSE_INLINE ~font_asset() {
      TTF_CloseFont(font.data);
    }

    font_instance font;
  };
}