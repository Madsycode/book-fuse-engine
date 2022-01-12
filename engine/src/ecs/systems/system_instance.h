#pragma once
#include "core/dispatcher.h"
#include "ecs/registry.h"

namespace fuse::ecs {
  struct system_instance {
    FUSE_INLINE virtual ~system_instance() = default;

    FUSE_INLINE void prepare(dispatcher* dp, registry* rg, SDL_Renderer *rd) {
      this->_dispatcher = dp;
      this->_registry = rg;
      this->_renderer = rd;
      this->init();
    }

    FUSE_INLINE virtual void update(float) {}
    FUSE_INLINE virtual void start() {}
    FUSE_INLINE virtual void init() {}

  protected:
    SDL_Renderer* _renderer = NULL;
    dispatcher* _dispatcher = NULL;    
    registry* _registry = NULL;
  };

  using system_list = std::vector<ecs::system_instance *>;
}