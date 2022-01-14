#pragma once
#include "asset_instance.h"

namespace fuse {
  struct font_instance {
    TTF_Font *data = NULL;
    int size = 16;
  };

  struct font_asset : asset_instance {
    FUSE_INLINE font_asset(const std::string& path, const std::string& name, const font_instance& data) {
      this->id = generate_uuid();
      this->font = data;
      this->name = name;
      this->path = path;
    }

    FUSE_INLINE font_asset() = default;

    FUSE_INLINE ~font_asset() {
      TTF_CloseFont(font.data);
    }

    font_instance font;
  };
}