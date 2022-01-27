#pragma once
#include "asset_instance.h"

namespace fuse {
  struct animation_instance {
    std::vector<asset_id> frames;
    int speed = 100;
  };

  struct animation_asset : asset_instance {
    FUSE_INLINE animation_asset(const std::string& name, const animation_instance& data) {
      this->animation = data;
      this->name = name;
    }

    FUSE_INLINE animation_asset() = default;

    animation_instance animation;
  };

  
}