#pragma once
#include "ecs/systems/tilemap_renderer_system.h"
#include "ecs/systems/sprite_renderer_system.h"
#include "ecs/systems/text_renderer_system.h"
#include "ecs/systems/animation_system.h"
#include "ecs/systems/rigidbody_system.h"
#include "ecs/systems/collision_system.h"

namespace fuse {
  struct scene_instance {
    FUSE_INLINE scene_instance(SDL_Renderer *rd, dispatcher *dp): _renderer(rd), _dispatcher(dp) {
      register_system<ecs::rigidbody_system>();
      register_system<ecs::collision_system>();
      register_system<ecs::tilemap_renderer_system>();
      register_system<ecs::sprite_renderer_system>();
      register_system<ecs::animation_system>();
      register_system<ecs::text_renderer_system>();
    }

    FUSE_INLINE ~scene_instance() {
      _registry.clear();
      _systems.clear();
      _assets.clear();
    }

    FUSE_INLINE void update(float dt) {
      SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);      
      for (auto& sys : _systems) { sys->update(dt); }
      this->_registry.refresh();      
    }

  FUSE_INLINE void start() {
    auto obj1 = _assets.load_texture("resource/obj1.png", "o1", _renderer)->id;
    auto obj2 = _assets.load_texture("resource/obj2.png", "o2", _renderer)->id;

    auto entity1 = _registry.add_entity();
    _registry.add_component<ecs::transform_component>(entity1);
    auto& body1 = _registry.add_component<ecs::rigidbody_component>(entity1).body;
    body1.apply_force_x(50);
    body1.gravity_scale = 0;
    _registry.add_component<ecs::sprite_component>(entity1, obj1);
    _registry.add_component<ecs::collider_component>(entity1);

    auto entity2 = _registry.add_entity();
    _registry.add_component<ecs::transform_component>(entity2).transform.translate.x = 500;
      auto& body2 = _registry.add_component<ecs::rigidbody_component>(entity2).body;
      body2.apply_force_x(-50);
      body2.gravity_scale = 0;
    _registry.add_component<ecs::sprite_component>(entity2, obj2);
    _registry.add_component<ecs::collider_component>(entity2);

    // start systems
    for (auto& sys : _systems) { sys->start(); }
  }

    template <typename T>
    FUSE_INLINE void register_system() {
      FUSE_STATIC_ASSERT(std::is_base_of<ecs::system, T>::value);
      auto system = new T();
      system->prepare(_dispatcher, &_registry, _renderer, &_assets);
      this->_systems.push_back(system);
    }

    private:
      dispatcher* _dispatcher = NULL;
      SDL_Renderer* _renderer = NULL;
      ecs::system_list _systems;
      ecs::registry _registry;
      asset_registry _assets;
  };
}