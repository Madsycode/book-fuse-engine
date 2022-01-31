#pragma once
#include "entity.h"

namespace fuse::ecs {
  struct system {
    FUSE_INLINE virtual ~system() = default;

    FUSE_INLINE void prepare(registry* rg, SDL_Renderer* rd) {
      this->_registry = rg;
      this->_renderer = rd;
    }
    
    template<typename T> 
    FUSE_INLINE auto view() {
      std::vector<entity> entities;
      for(auto& e : _registry->view<T>()) {
        entities.push_back(entity(e, _registry));
      }
      return entities;
    }

    FUSE_INLINE virtual void update(float) {}
    FUSE_INLINE virtual void start() {}

  protected:
    SDL_Renderer* _renderer = NULL;
    registry* _registry = NULL;
  };
}