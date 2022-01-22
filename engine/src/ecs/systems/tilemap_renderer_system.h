#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct tilemap_renderer_system : system {
    FUSE_INLINE void update(float) {
      for (auto& entity : _registry->view<tilemap_component>()) {
        auto& tr = _registry->get_component<transform_component>(entity).transform;
        auto& id = _registry->get_component<tilemap_component>(entity).tilemap;
        auto& tilemap = _assets->get<tilemap_asset>(id)->tilemap;

        // render current tilemap's tiles
        for (auto& t_entity : _registry->view<tile_component>()) {
          // check if tile belongs to current tilemap
          auto& tile = _registry->get_component<tile_component>(t_entity);
          if (tile.tilemap != id && !tilemap.tilesets.count(tile.tileset)) {
            continue; 
          }
          // render tile
          float x = tile.x + tr.translate.x;
          float y = tile.y + tr.translate.y;                  
          draw_tile(tile.tileset, x, y, tile.row, tile.col, tilemap.tilesize);
        }
      }
    }

  private:
    FUSE_INLINE void draw_tile(asset_id tileset_id, float x, float y, int row, int col, int size) {
      SDL_FRect dst_rect = { x * size, y * size, size, size};
      SDL_Rect src_rect = {row * size, col * size, size, size};
      auto& tileset = _assets->get<texture_asset>(tileset_id)->texture;
      SDL_RenderCopyExF(_renderer, tileset.data, &src_rect, &dst_rect, 0, NULL, tileset.flip);
      // render rectangle
      SDL_RenderDrawRectF(_renderer, &dst_rect);
    }
  };
}