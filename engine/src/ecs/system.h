#pragma once
#include "ecs/components/behaviour.h"
#include "ecs/components/graphics.h"
#include "ecs/components/physics.h"
#include "ecs/components/common.h"

#include "assets/registry.h"
#include "ecs/registry.h"

namespace fuse::ecs {
  struct system {
    FUSE_INLINE virtual ~system() = default;

    FUSE_INLINE void prepare(registry* rg, SDL_Renderer *rd, asset_registry* as) {
      this->_registry = rg;
      this->_renderer = rd;
      this->_assets = as;
    }

    FUSE_INLINE virtual void update(float) {}
    FUSE_INLINE virtual void start() {}

  protected:
    SDL_Renderer* _renderer = NULL;
    asset_registry* _assets = NULL;  
    registry* _registry = NULL;
  };
}