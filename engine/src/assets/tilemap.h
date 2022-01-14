#pragma once

#include "asset_instance.h"

namespace fuse {
  struct tilemap_instance {
    std::set<asset_id> tilesets;
    int column_count = 0;
    int row_count = 0;
    int tilesize = 0;
  };

  struct tilemap_asset : asset_instance {
    FUSE_INLINE tilemap_asset(const std::string& path, const std::string& name, const tilemap_instance& data) {
      this->id = generate_uuid();
      this->tilemap = data;
      this->path = path;
    }

    FUSE_INLINE tilemap_asset() = default;

    tilemap_instance tilemap;
  };
}