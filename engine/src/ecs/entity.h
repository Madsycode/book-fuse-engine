#pragma once
#include "registry.h"

namespace fuse::ecs {
  struct entity {
    FUSE_INLINE entity(registry* r): _registry(r) { _entity = r->add_entity(); }
    FUSE_INLINE entity(entityid e, registry* r): _registry(r), _entity(e) {}
    FUSE_INLINE operator entityid() { return _entity; }
    FUSE_INLINE entity() = default;

    template<typename T, typename... Args>
    FUSE_INLINE T& add_component(Args&&... args) {
      return _registry->add_component<T>(_entity, std::forward<Args>(args)...);
    }

    template<typename T>
    FUSE_INLINE T& get_component() {
      return _registry->get_component<T>(_entity);
    }

    template<typename T>
    FUSE_INLINE void remove_component() {
      _registry->remove_component<T>(_entity);
    }

    template<typename T>
    FUSE_INLINE bool has_component() {
      return _registry->has_component<T>(_entity);
    }

    FUSE_INLINE bool is_alive() {
			return _registry && _registry->is_alive(_entity);
		}

    FUSE_INLINE entityid id() { return _entity; }
    
    FUSE_INLINE void destroy() { _registry->destroy(_entity); }

  private:
    entityid _entity = INVALID_ID;
    registry* _registry = NULL;
  };
}