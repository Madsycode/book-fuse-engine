#pragma once 
#include "physics/rigidbody2.h"

namespace fuse::ecs {    
  struct rigidbody_component {
    FUSE_INLINE rigidbody_component(const rigidbody_component&) = default;
    FUSE_INLINE rigidbody_component() = default;
    bool disabled = false;
    rigidbody2 body;  
  };

  struct collider_component {
    FUSE_INLINE collider_component(const collider_component&) = default;
    FUSE_INLINE collider_component() = default;
    SDL_FRect collider = { 0.0f, 0.0f, 64.0f, 64.0f };   
    bool disabled = false;  
  };

  struct tag_component {
    FUSE_INLINE tag_component(const tag_component&) = default;
    FUSE_INLINE tag_component() = default;
    std::string tag;
  };
}