#pragma once
#include "ecs/systems/text_renderer_system.h"
#include "ecs/systems/frame_animation_system.h"
#include "ecs/systems/sprite_renderer_system.h"
#include "ecs/systems/tilemap_renderer_system.h"

namespace fuse::ecs {
  struct scene {
    FUSE_INLINE scene(SDL_Renderer* rd): _renderer(rd) {
      register_system<ecs::tilemap_renderer_system>();
      register_system<ecs::frame_animation_system>();
      register_system<ecs::sprite_renderer_system>();
      register_system<ecs::text_renderer_system>();
    }

    FUSE_INLINE ~scene() {
      for (auto& s : _systems) { FUSE_DELETE(s); }
      _registry.clear();
      _systems.clear();
    }

    FUSE_INLINE ecs::entity add_entity(const std::string& name) {
      auto entity = ecs::entity(&_registry);
      entity.add_component<info_component>().name = name;
      entity.add_component<transform_component>();
      return entity;
    }

    FUSE_INLINE void update(float dt) {
      SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
      for (auto& sys : _systems) { sys->update(dt); }
    }

FUSE_INLINE void start() {
  // load texture
  auto ts = _assets.load_texture("assets/tex.png", "", _renderer);

  // create tilemap asset
  auto tm = _assets.add<tilemap_asset>("tm");
  tm->instance.tilesets.insert(ts->id);
  tm->instance.col_count = 16;
  tm->instance.row_count = 8;
  tm->instance.tilesize = 64;

  // add tilemap entity
  auto& tilemap = add_entity("tilemap");
  tilemap.add_component<tilemap_component>().tilemap = tm->id;

  // turn image into multiple entities with tiles
  for (int col = 0; col < tm->instance.col_count; col++) {
    for (int row = 0; row <  tm->instance.row_count; row++) {
      ecs::entity e = add_entity("tile");
      auto& tile = e.add_component<tile_component>();
      tile.tileset = ts->id;
      tile.tilemap = tm->id;
      tile.offset_x = col;
      tile.offset_y = row;
      tile.row = col;
      tile.col = row;
    }  
  }

  // start systems
  for (auto& sys : _systems) { sys->start(); }
}

    template <typename T>
    FUSE_INLINE void register_system() {
      FUSE_STATIC_ASSERT(std::is_base_of<ecs::system, T>::value);
      auto new_system = new T();
      new_system->prepare(&_registry, _renderer, &_assets);
      this->_systems.push_back(new_system);
    }

  private:
    std::vector<ecs::system*> _systems;
    SDL_Renderer* _renderer = NULL;
    ecs::registry _registry;
    asset_registry _assets;
  };
}