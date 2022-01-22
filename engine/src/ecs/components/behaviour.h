#pragma once
#include "scripts/fuse_script.h"

namespace fuse::ecs {
  struct script_component {
    FUSE_INLINE script_component(const script_component&) = default;
    FUSE_INLINE script_component() = default;

    template<typename T>
    FUSE_INLINE void bind() { 
      instantiate = [&](auto entiy, auto assets) { 
        script = std::make_shared<T>();         
        script->init(entiy, assets);
      };    
    }   

    std::function<void(const entity&, asset_registry*)> instantiate;
    std::shared_ptr<fuse_script> script;
  };
}