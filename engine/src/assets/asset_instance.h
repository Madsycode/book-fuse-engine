#pragma once

namespace fuse {
  using asset_id = uuid64;
  
  struct asset_instance {
    FUSE_INLINE virtual ~asset_instance() = default;
    asset_id id = generate_uuid();
    std::string name;
  };

  using asset_list = std::vector<asset_instance*>;
}
