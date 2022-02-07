#pragma once
#include "asset.h"

namespace fuse {
  struct tilemap {
    std::set<asset_id> tilesets;
    int col_count = 0;
    int row_count = 0;
    int tilesize = 0;
  };

  struct tilemap_asset : asset {
    FUSE_INLINE tilemap_asset() = default;
    tilemap instance;
  };
}