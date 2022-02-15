#pragma once
#include "ecs.h"

namespace fuse::ecs {
  struct registry {
    FUSE_INLINE void clear() {
      for (auto& [_, a] : _components) { FUSE_DELETE(a); }
      _components.clear();
      _signatures.clear();
      _nextid = INVALID_ID;
    }

    FUSE_INLINE entityid add_entity() {
      entityid entity = (++_nextid);
      _signatures[entity] = signature();
      return entity;
    }

    FUSE_INLINE void destroy(entityid e) {
      for (auto& [_, a] : _components) {
        a->erase(e);
      }
      _signatures.erase(e);
    }

    FUSE_INLINE bool is_alive(entityid e) {
			return _signatures.count(e);
		}

    template <typename T>
    FUSE_INLINE entitylist view() {
      entitylist list;
      for (auto& [entity, sig] : _signatures) {
        if (sig.count(type_id<T>())) {
          list.insert(entity);
          continue;
        }
      }
      return list;
    }

    template <typename T>
    FUSE_INLINE T& get_component(entityid e) {
      FUSE_ASSERT(_signatures.count(e) && "out of range!");
      return get_component_array<T>()->get(e);
    }

    template <typename T, typename... Args>
    FUSE_INLINE T &add_component(entityid e, Args&&...args) {
      FUSE_ASSERT(_signatures.count(e) && "out of range!");
      auto data = T(std::forward<Args>(args)...);
      _signatures[e].insert(type_id<T>());
      return get_component_array<T>()->push(e, data);
    }

    template <typename T>
    FUSE_INLINE void remove_component(entityid e) {
      _signatures.at(e).erase(type_id<T>());
      get_component_array<T>()->erase(e);
    }

    template <typename T>
    FUSE_INLINE bool has_component(entityid e) {
      return _signatures.count(e) && _signatures.at(e).count(type_id<T>());
    }

    // ++

    template <typename T>
    FUSE_INLINE component_array<T>* get_component_array() {
      componentid type = type_id<T>();
      if(_components.count(type)) {
        return static_cast<component_array<T>*>(_components.at(type));
      }
      auto array = new component_array<T>();
      _components[type] = array;
      return array;
    }

  private:
    std::unordered_map<componentid, array_instance*> _components;
    std::unordered_map<entityid, signature> _signatures;
    entityid _nextid = INVALID_ID;
  };
}