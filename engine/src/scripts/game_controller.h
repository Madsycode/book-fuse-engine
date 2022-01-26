#pragma once

#include "script_instance.h"

namespace fuse {
  struct game_controller : script_instance {
    FUSE_INLINE void on_start() {
      
    }

    FUSE_INLINE void on_update(float dt) {
      if(game_over) { return; } 

      // check if player is dead
      if(find_entity("player").get_component<ecs::rigidbody_component>().disabled) {      
        // stop ground scrolling animation
        find_entity("ground").get_component<ecs::rigidbody_component>().disabled = true;      
        // show game over text
        auto& tx = find_entity("score").add_component<ecs::text_component>();
        tx.font = get_asset<font_asset>("font")->id;
        tx.text = "GAME OVER";        
        game_over = true;   
        return;    
      }

      update_pipes();
      time += dt;

      if(time > 2.5f) {               
        float space = math::random(60, 150);
        spawn_pipe(SCREEN_WIDTH, -space);         
        spawn_pipe(SCREEN_WIDTH, SCREEN_HEIGHT/2 + space);
        time = 0.0f;
      } 
    }

    FUSE_INLINE void spawn_pipe(float x, float y) {
      auto pipe = add_entity();     
      auto& tr = pipe.get_component<ecs::transform_component>();
      tr.transform.translate = vec2f(x, y);      
      // add rigidbody
      auto& rb = pipe.add_component<ecs::rigidbody_component>();   
      rb.body.gravity_scale = 0.0f;
      rb.body.velocity.x = -speed;
      // add sprite
      auto pipe_sprite = get_asset<texture_asset>("pipe");
      auto& sp = pipe.add_component<ecs::sprite_component>();    
      sp.flip = (y < 0)? SDL_FLIP_NONE : SDL_FLIP_VERTICAL;
      sp.sprite = pipe_sprite->id;
      // add collider
      auto& cl = pipe.add_component<ecs::collider_component>();
      cl.collider = { 
        tr.transform.translate.x,
        tr.transform.translate.y,
        (float)pipe_sprite->texture.width,
        (float)pipe_sprite->texture.height 
      };     
      pipes.push_back(pipe);         
    }

    FUSE_INLINE void update_pipes() {
      auto it = pipes.begin();
      while (it != pipes.end()) {       
        // remove dead pipe
        auto& pipe = (*it);        
        if (!pipe.is_alive()) {
          it = pipes.erase(it);          
          continue;
        }       
        // destroy if out of the screen and set score
        auto& tr = pipe.get_component<ecs::transform_component>().transform;
        if(tr.translate.x < -50) { 
          auto& tx = find_entity("score").get_component<ecs::text_component>();
          tx.text = "Score: " + std::to_string(++score);
          pipe.destroy(); 
        }      
        it++;
      }         
    }

  private:     
    std::vector<ecs::entity> pipes;
    bool game_over = false;
    float speed = 100.0f;
    float time = 0.0f;
    int score = 0;
  };
}