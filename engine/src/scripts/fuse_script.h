#pragma once
#include "ecs/components/graphics.h"
#include "ecs/components/physics.h"
#include "ecs/components/common.h"
#include "ecs/entity.h"

#include "assets/asset_registry.h"
#include "core/inputs.h"

namespace fuse {
  struct fuse_script {
    FUSE_INLINE virtual ~fuse_script() = default;

    FUSE_INLINE void init(const ecs::entity& e, asset_registry* a) {
      this->_assets = a;
      this->_entity = e;
      this->on_create();
    }

    FUSE_INLINE void virtual on_collision(ecs::entity) {}
    FUSE_INLINE void virtual on_update(float) {}
    FUSE_INLINE void virtual on_destroy() {}
    FUSE_INLINE void virtual on_create() {}
    FUSE_INLINE void virtual on_start() {}

  protected:
    template<typename T, typename... Args>
    FUSE_INLINE T& add_component(Args&&... args) {
        return _entity.add_component<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    FUSE_INLINE T& get_component() {
      return _entity.get_component<T>();
    }

    template<typename T>
    FUSE_INLINE void remove_component() {
      return _entity.remove_component<T>();
    }

    template<typename T>
    FUSE_INLINE bool has_component() {
      return _entity.has_component<T>();
    }

    FUSE_INLINE void destroy() {
			_entity.destroy();
		}

  private:
    asset_registry* _assets = NULL;
    ecs::entity _entity;
  };
} 
