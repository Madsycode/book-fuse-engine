#pragma once
#include "script_instance.h"

namespace fuse {
  struct player_controller : script_instance {   
    FUSE_INLINE void on_start() { 
     
    }

    FUSE_INLINE void on_collision(ecs::entity e) { 
      play_audio(get_component<ecs::audio_component>().audio, 40, 0);
      get_component<ecs::collider_component>().disabled = true;
      auto& an = get_component<ecs::animation_component>();
      an.animation = get_asset<animation_asset>("hurt")->id;
      auto& rb = get_component<ecs::rigidbody_component>();
      rb.disabled = true;
    }

    FUSE_INLINE void on_update(float dt) {      
      auto& rb = get_component<ecs::rigidbody_component>();
      if(inputs::is_pressed(SDL_SCANCODE_SPACE)) {
        rb.body.set_force_y(-1000.0f);
      }  
      else {
        rb.body.set_force_y(0.0f);
      }            
    }
  };
}