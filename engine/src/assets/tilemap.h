#pragma once

#include "asset_instance.h"

namespace fuse {
  struct tilemap_instance {
    std::set<asset_id> tilesets;
    int col_count = 0;
    int row_count = 0;
    int tilesize = 0;
  };

  struct tilemap_asset : asset_instance {
    FUSE_INLINE tilemap_asset(const std::string& name, const tilemap_instance& data) {
      this->tilemap = data;
      this->name = name;
    }

    FUSE_INLINE tilemap_asset() = default;

    tilemap_instance tilemap;
  };
}