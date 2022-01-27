#pragma once
#include "asset.h"

namespace fuse {
  struct animation {
    std::vector<asset_id> frames;
    int speed = 100;
  };

  struct animation_asset : asset {
    FUSE_INLINE animation_asset(const std::string& name, const animation& data) {
      this->animation = data;
      this->name = name;
    }

    FUSE_INLINE animation_asset() = default;

    animation animation;
  };

  
}