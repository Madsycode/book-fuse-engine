#pragma once

#include "ecs.h"

namespace fuse::ecs {
  struct registry {
    FUSE_INLINE void clear() {
      for (auto& [_, a] : _components) { FUSE_DELETE(a); }
      _components.clear();
      _signatures.clear();
    }

    FUSE_INLINE entity_id add_entity() {
      static entity_id _nextid = INVALID_ID;
      _nextid++;
      _signatures[_nextid] = signature();
      return _nextid;
    }

    FUSE_INLINE void destroy(entity_id entity) {
      for (auto& [_, a] : _components) {
        a->erase(entity);
      }
      _signatures.erase(entity);
    }

    FUSE_INLINE bool is_alive(entity_id entity) {
			return _signatures.count(entity);
		}

    template <typename T>
    FUSE_INLINE entity_list view() {
      entity_list list;
      for (auto& [entity, sig] : _signatures) {
        if (sig.count(get_typeid<T>())) {
          list.insert(entity);
          continue;
        }
      }
      return list;
    }

    template <typename T>
    FUSE_INLINE T& get_component(entity_id entity) {
      FUSE_ASSERT(_signatures.count(entity) && "out of range!");
      return get_component_array<T>()->get(entity);
    }

    template <typename T, typename... Args>
    FUSE_INLINE T &add_component(entity_id entity, Args&&...args) {
      FUSE_ASSERT(_signatures.count(entity) && "out of range!");
      auto data = T(std::forward<Args>(args)...);
      _signatures[entity].insert(get_typeid<T>());
      return get_component_array<T>()->push(entity, data);
    }

    template <typename T>
    FUSE_INLINE void remove_component(entity_id entity) {
      _signatures.at(entity).erase(get_typeid<T>());
      get_component_array<T>()->erase(entity);
    }

    template <typename T>
    FUSE_INLINE bool has_component(entity_id entity) {
      return _signatures.count(entity) && _signatures.at(entity).count(get_typeid<T>());
    }

    // ++

    template <typename T>
    FUSE_INLINE component_array<T>* get_component_array() {
      const type_id type = get_typeid<T>();
      if(_components.count(type)) {
        return static_cast<component_array<T>*>(_components.at(type));
      }
      auto array = new component_array<T>();
      _components[get_typeid<T>()] = array;
      return array;
    }

  private:
    std::unordered_map<type_id, array_instance*> _components;
    std::unordered_map<entity_id, signature> _signatures;
  };
}