#pragma once
#include "ecs/systems/test_system.h"
#include "assets/asset_registry.h"

namespace fuse {
  struct scene_instance {
    FUSE_INLINE scene_instance(SDL_Renderer *rd, dispatcher *dp): _renderer(rd), _dispatcher(dp) {
      // register systems
      this->register_system<ecs::test_system>();
    }

    FUSE_INLINE ~scene_instance() {
      _registry.clear();
      _systems.clear();
      _assets.clear();
    }

    FUSE_INLINE void update(float dt) {
      for (auto& sys : _systems) { sys->update(dt); }
      _registry.refresh();
    }

    FUSE_INLINE void start() {
      auto entity = _registry.add_entity();
      _registry.add_component<ecs::transform_component>(entity);

      auto sprite = _assets.load_texture("resource/sprite.png", "sprite", _renderer);
      auto font = _assets.load_font("resource/roboto.ttf", "myfont", 16);
      auto audio = _assets.load_audio("resource/song.mp3", "mysong", 50);

      auto tilemap = _assets.add<tilemap_asset>("resource/tilemap.a", "jungle");
      auto animation = _assets.add<animation_asset>("resource/run.a", "run");

      auto id = _assets.get_id<texture_asset>("sprite"); 
      auto tx = _assets.get<texture_asset>(id);
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