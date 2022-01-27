#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct collision_system : system {
    FUSE_INLINE void start() {
      for (auto& entity : _registry->view<collider_component>()) {
        auto& col = _registry->get_component<collider_component>(entity);
        auto& tr = _registry->get_component<transform_component>(entity);
        col.collider.x = tr.transform.translate.x;
        col.collider.y = tr.transform.translate.y;
      }
    }

    FUSE_INLINE void update(float) {
      // get entity list
      auto entities = _registry->view<collider_component>();

      for (auto& entity : entities) {
        auto& tr = _registry->get_component<transform_component>(entity);               
        auto& c1 = _registry->get_component<collider_component>(entity);

        // update box collider offset
        c1.collider.x = tr.transform.translate.x;
        c1.collider.y = tr.transform.translate.y;

        // continue if disabled
        if(c1.disabled) { continue; }

        // check collision with others
        for (auto& other : entities) {
          if (other == entity) { continue; }
          auto& c2 = _registry->get_component<collider_component>(other);

          // call script oncllision() function
          if (check_collision(c1.collider, c2.collider)) {
            if(_registry->has_component<script_component>(entity)) {              
              _registry->get_component<script_component>(entity).instance->on_collision(ecs::entity(other, _registry));
            }
          }          
        }
      }
    }

  private:
    bool check_collision(const SDL_FRect& a, const SDL_FRect& b) {
      return ((a.x < b.x + b.w) && (a.x + a.w > b.x)) &&
             ((a.y < b.y + b.h) && (a.y + a.h > b.y));
    }
  };
}