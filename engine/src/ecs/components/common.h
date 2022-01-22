#pragma once
#include "math/transform2.h"

namespace fuse::ecs {
  struct info_component {
    FUSE_INLINE info_component(const std::string& name, uuid64 uuid): name(name), uuid(uuid) {}
    FUSE_INLINE info_component(const info_component&) = default;
    FUSE_INLINE info_component() = default;
    uuid64 uuid = INVALID_ID;
    std::string name;
  };

  struct transform_component {
    FUSE_INLINE transform_component(const transform_component &) = default;
    FUSE_INLINE transform_component() = default;
    transform2 transform;
  };
}