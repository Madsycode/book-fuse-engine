#pragma once
#include "script_instance.h"

namespace fuse {
  struct player_controller : script_instance {   
    FUSE_INLINE void on_collision(ecs::entity e) { 
      play_audio("boom");
      auto& sp = get_component<ecs::sprite_component>();
      sp.sprite = get_asset<texture_asset>("dead")->id;
      get_component<ecs::collider_component>().disabled = true;
      get_component<ecs::rigidbody_component>().disabled = true;
    }

    FUSE_INLINE void on_update(float dt) {      
      auto& rb = get_component<ecs::rigidbody_component>();
      if(inputs::is_key(SDL_SCANCODE_SPACE)) {
        rb.body.set_force_y(-1000.0f);
      }  
      else {
        rb.body.set_force_y(0.0f);
      }            
    }
  };
}