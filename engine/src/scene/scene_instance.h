#pragma once
#include "ecs/systems/test_system.h"

namespace fuse {
  struct scene_instance {
    FUSE_INLINE scene_instance(SDL_Renderer *rd, dispatcher *dp): _renderer(rd), _dispatcher(dp) {
      // register components array
      _registry.register_component<ecs::transform_component>();
      _registry.register_component<ecs::camera_component>();
      _registry.register_component<ecs::info_component>();

      // register systems
      this->register_system<ecs::test_system>();
    }

    FUSE_INLINE ~scene_instance() {
      _systems.clear();
    }

    FUSE_INLINE void update(float dt) {
      for (auto& sys : _systems) { sys->update(dt); }
      _registry.refresh();
    }

    FUSE_INLINE void start() {
      // add entity1
      ecs::entity_id entity1 = _registry.add_entity();
      _registry.add_component<ecs::transform_component>(entity1);

      // add entity 2
      ecs::entity_id entity2 = _registry.add_entity();
      _registry.add_component<ecs::info_component>(entity2);
      _registry.add_component<ecs::transform_component>(entity2);
    }

    template <typename T>
    FUSE_INLINE void register_system() {
      FUSE_STATIC_ASSERT(std::is_base_of<ecs::system_instance, T>::value);
      auto system = new T();
      system->prepare(_dispatcher, &_registry, _renderer);
      this->_systems.push_back(system);
    }

    private:
      dispatcher* _dispatcher = NULL;
      SDL_Renderer* _renderer = NULL;
      ecs::system_list _systems;
      ecs::registry _registry;
  };
}