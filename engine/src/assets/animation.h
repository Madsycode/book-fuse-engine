#pragma once
#include "asset_instance.h"

namespace fuse {
  struct animation_instance {
    std::vector<asset_id> frames;
    int frame_count = 0;
    int speed = 100;
  };

  struct animation_asset : asset_instance {
    FUSE_INLINE animation_asset(const std::string &path, const std::string &name, const animation_instance &data) {
      this->id = generate_uuid();
      this->animation = data;
      this->name = name;
      this->path = path;
    }

    FUSE_INLINE animation_asset() = default;

    animation_instance animation;
  };
}