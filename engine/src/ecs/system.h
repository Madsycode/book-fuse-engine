#pragma once
#include "ecs/components/graphics.h"
#include "ecs/components/physics.h"
#include "ecs/components/common.h"

#include "assets/asset_registry.h"
#include "core/dispatcher.h"
#include "ecs/registry.h"

namespace fuse::ecs {
  struct system {
    FUSE_INLINE virtual ~system() = default;

    FUSE_INLINE void prepare(dispatcher* dp, registry* rg, SDL_Renderer *rd, asset_registry* as) {
      this->_dispatcher = dp;
      this->_registry = rg;
      this->_renderer = rd;
      this->_assets = as;
      this->init();
    }

    FUSE_INLINE virtual void update(float) {}
    FUSE_INLINE virtual void start() {}
    FUSE_INLINE virtual void init() {}

  protected:
    SDL_Renderer* _renderer = NULL;
    asset_registry* _assets = NULL;
    dispatcher* _dispatcher = NULL;    
    registry* _registry = NULL;
  };

  using system_list = std::vector<ecs::system*>;
}