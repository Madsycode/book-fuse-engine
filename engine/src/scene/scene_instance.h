#pragma once
#include "ecs/systems/tilemap_renderer_system.h"
#include "ecs/systems/sprite_renderer_system.h"
#include "ecs/systems/text_renderer_system.h"
#include "ecs/systems/animation_system.h"

namespace fuse {
  struct scene_instance {
FUSE_INLINE scene_instance(SDL_Renderer *rd, dispatcher *dp): _renderer(rd), _dispatcher(dp) {
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
  auto tileset = _assets.load_texture("resource/ts.jpg", "ts", _renderer);
  auto asset = _assets.add<tilemap_asset>("path", "name");
  asset->tilemap.tilesets.insert(tileset->id);
  asset->tilemap.col_count = 16;
  asset->tilemap.row_count = 8;
  asset->tilemap.tilesize = 64;

  ecs::entity_id entity = _registry.add_entity();
  _registry.add_component<ecs::transform_component>(entity);
  _registry.add_component<ecs::tilemap_component>(entity, asset->id);

  for (int col = 0; col < asset->tilemap.col_count; col++) {
    for (int row = 0; row <  asset->tilemap.row_count; row++) {
      auto tile_entity = _registry.add_entity();
      auto& tile = _registry.add_component<ecs::tile_component>(tile_entity);
      tile.tileset = tileset->id;
      tile.tilemap = asset->id;
      tile.row = col;
      tile.col = row;
      tile.x = col;
      tile.y = row;
    }
  }
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