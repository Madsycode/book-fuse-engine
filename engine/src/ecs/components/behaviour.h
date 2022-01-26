#pragma once
#include "scripts/script_instance.h"

namespace fuse::ecs {
  struct script_component {
    FUSE_INLINE script_component(const script_component&) = default;
    FUSE_INLINE script_component() = default;

    template<typename T>
    FUSE_INLINE void bind() { 
      allocate = [](const script_props& props) { 
        auto script = static_cast<script_instance*>(new T());         
        script->init(props);
        return script;
      };    
    }   

    script_instance* instance = NULL;
    script_allocator allocate;
  };

  struct audio_component {
    FUSE_INLINE audio_component(const audio_component&) = default;
    FUSE_INLINE audio_component() = default;
    asset_id audio;
  };
}