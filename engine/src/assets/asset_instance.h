#pragma once

namespace fuse {
  using asset_id = uuid64;
  
  struct asset_instance {
    FUSE_INLINE virtual ~asset_instance() = default;
    asset_id id = INVALID_ID;
    std::string name;
    std::string path;
  };

  using asset_list = std::vector<asset_instance*>;
}
