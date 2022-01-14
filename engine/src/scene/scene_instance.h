#pragma once
#include "ecs/systems/sprite_renderer_system.h"
#include "ecs/systems/text_renderer_system.h"

namespace fuse {
  struct scene_instance {
    FUSE_INLINE scene_instance(SDL_Renderer *rd, dispatcher *dp): _renderer(rd), _dispatcher(dp) {
      this->register_system<ecs::sprite_renderer_system>();
      this->register_system<ecs::text_renderer_system>();
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
      auto font = _assets.load_font("resource/roboto.ttf", "roboto", 30);

      ecs::entity_id entity = _registry.add_entity();
      _registry.add_component<ecs::transform_component>(entity);
      _registry.add_component<ecs::text_component>(entity, font->id, "This is a text!");
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