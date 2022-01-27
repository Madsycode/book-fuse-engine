#pragma once

namespace fuse {
  using asset_id = uuid64;
  
  struct asset {
    FUSE_INLINE virtual ~asset() = default;
    asset_id id = generate_uuid();
    std::string name;
  };

  using asset_list = std::vector<asset*>;
}
