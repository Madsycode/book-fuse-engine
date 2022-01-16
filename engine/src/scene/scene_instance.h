#pragma once
#include "ecs/systems/sprite_renderer_system.h"
#include "ecs/systems/text_renderer_system.h"
#include "ecs/systems/animation_system.h"

namespace fuse {
  struct scene_instance {
    FUSE_INLINE scene_instance(SDL_Renderer *rd, dispatcher *dp): _renderer(rd), _dispatcher(dp) {
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
      auto f1 = _assets.load_texture("resource/f1.png", "f1", _renderer);
      auto f2 = _assets.load_texture("resource/f2.png", "f2", _renderer);
      auto f3 = _assets.load_texture("resource/f3.png", "f3", _renderer);

      auto asset = _assets.add<animation_asset>("path", "name");
      asset->animation.frames.push_back(f1->id);
      asset->animation.frames.push_back(f2->id);
      asset->animation.frames.push_back(f3->id);
      asset->animation.frame_count = 3;

      ecs::entity_id entity = _registry.add_entity();
      _registry.add_component<ecs::transform_component>(entity);
      _registry.add_component<ecs::animation_component>(entity, asset->id);
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