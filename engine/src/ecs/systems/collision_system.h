#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct collision_system : system {
    FUSE_INLINE void start() {
      for (auto& e : view<collider_component>()) {
        auto& tr = e.get_component<transform_component>();
        auto& col = e.get_component<collider_component>();
        col.collider.x = tr.translate.x;
        col.collider.y = tr.translate.y;
      }
    }

FUSE_INLINE void update(float) {
  // get entity list
  auto entities = view<collider_component>();

  for (auto& e : entities) {
    auto& tr = e.get_component<transform_component>();               
    auto& c1 = e.get_component<collider_component>();

    // update box collider offset
    c1.collider.x = tr.translate.x;
    c1.collider.y = tr.translate.y;

    // continue if disabled
    if(c1.disabled) { continue; }

    // check collision with others
    for (auto& o : entities) {
      if (o == e) { continue; }
      auto& c2 = o.get_component<collider_component>();
      if (check_collision(c1.collider, c2.collider)) {
        if(e.has_component<script_component>()){
          auto& script = e.get_component<script_component>();
          script.instance->on_collision(o);
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