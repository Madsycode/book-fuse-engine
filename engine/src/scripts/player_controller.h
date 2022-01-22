#pragma once
#include "fuse_script.h"

namespace fuse {
  struct player_controller : fuse_script {
    FUSE_INLINE void on_create() { 
      FUSE_INFO("player_script created!");
    }

    FUSE_INLINE void on_start() { 
      FUSE_INFO("player_script started!");
    }

    FUSE_INLINE void on_collision(ecs::entity e) { 
      FUSE_INFO("colliding with: %d", e.id());
    }

    FUSE_INLINE void on_update(float dt) { 
      float speed = 300.0f;
      auto& tr = get_component<ecs::transform_component>().transform;

      // move left
      if(inputs::is_pressed(SDL_SCANCODE_A)) {
        tr.translate.x -= (speed * dt);
      }
      // move right
      if(inputs::is_pressed(SDL_SCANCODE_D)) {
        tr.translate.x += (speed * dt);
      }
      // move up
      if(inputs::is_pressed(SDL_SCANCODE_W)) {
        tr.translate.y -= (speed * dt);
      }
      // move down
      if(inputs::is_pressed(SDL_SCANCODE_S)) {
        tr.translate.y += (speed * dt);
      }      
    }
  };
}