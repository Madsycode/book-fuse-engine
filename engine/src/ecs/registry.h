#pragma once

#include "ecs.h"

namespace fuse::ecs {
  struct registry {
    FUSE_INLINE ~registry() {
      _destroyed_entities.clear();
      _components.clear();
      _signatures.clear();
    }

    FUSE_INLINE void refresh() {
      for (auto &entity : _destroyed_entities) {
        for (auto &[_, a] : _components) {
          a->erase(entity);
        }
        _signatures.erase(entity);
      }
      _destroyed_entities.clear();
    }

    FUSE_INLINE entity_id add_entity() {
      static entity_id _next_entityid = INVALID_ID;
      _next_entityid++;
      _signatures[_next_entityid] = signature();
      return _next_entityid;
    }

    FUSE_INLINE void destroy_entity(entity_id entity) {
      _destroyed_entities.insert(entity);
    }

    // ++

    FUSE_INLINE void view(entity_list &list, const signature &sig) {
      for (auto &[entity, signature] : _signatures) {
        bool has_signature = true;
        for (auto &type : sig) {
          if (!signature.count(type)) {
            has_signature = false;
            break;
          }
        }
        if (has_signature) { list.insert(entity); }
      }
    }

    template <typename T>
    FUSE_INLINE entity_list view() {
      entity_list list;
      for (auto &[entity, sig] : _signatures) {
        if (sig.count(get_typeid<T>())) {
          list.insert(entity);
          continue;
        }
      }
      return list;
    }

    FUSE_INLINE const entity_list& entities() {
      static entity_list entities;
      entities.clear();
      for (auto &[entity,_] : _signatures) {
        entities.insert(entity);
      }
      return entities;
    }

    // ++  

    template <typename T>
    FUSE_INLINE T &get_component(entity_id entity) {
      FUSE_ASSERT(_signatures.count(entity) && "out of range!");
      return find_component_array<T>()->get(entity);
    }

    template <typename T, typename... Args>
    FUSE_INLINE T &add_component(entity_id entity, Args &&...args) {
      FUSE_ASSERT(_signatures.count(entity) && "out of range!");
      _signatures[entity].insert(get_typeid<T>());
      return find_component_array<T>()->push(entity, T(std::forward<Args>(args)...));
    }

    template <typename T>
    FUSE_INLINE void remove_component(entity_id entity) {
      if (has_component<T>(entity)) {
        find_component_array<T>()->erase(entity);
        _signatures.at(entity).erase(get_typeid<T>());
      }
    }

    template <typename T>
    FUSE_INLINE bool has_component(entity_id entity) {
      return _signatures.count(entity) && _signatures.at(entity).count(get_typeid<T>());
    }

    // ++

    template <typename T>
    FUSE_INLINE void register_component() {
      type_id _typeid = get_typeid<T>();
      FUSE_ASSERT(!_components.count(_typeid) && "allready register!");
      _components.insert({_typeid, new component_array<T>()});
    }

    template <typename T>
    FUSE_INLINE component_array<T>* find_component_array() {
      type_id _typeid = get_typeid<T>();
      FUSE_ASSERT(_components.count(_typeid) && "array not found!");
      return static_cast<component_array<T> *>(_components.at(_typeid));
    }

  private:
    std::unordered_map<type_id, array_instance *> _components;
    std::unordered_map<entity_id, signature> _signatures;
    entity_list _destroyed_entities;
  };
}