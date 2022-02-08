#pragma once
#include "ecs/system.h"

namespace fuse::ecs {
  struct tilemap_renderer_system : system {
    FUSE_INLINE void update(float) {
      for (auto& e : view<tilemap_component>()) {
        auto& tr = e.get_component<transform_component>();
        auto& tm = e.get_component<tilemap_component>().tilemap;

        // get tilemap asset
        auto& tilemap = _assets->get<tilemap_asset>(tm)->instance;

        // render current tilemap's tiles
        for (auto& tile_entt : view<tile_component>()) {          
          // check if tile belongs to current tilemap
          auto& tile = tile_entt.get_component<tile_component>();
          if (tile.tilemap != tm && !tilemap.tilesets.count(tile.tileset)) {
            continue; 
          }

          // tile position
          float x = tile.offset_x + tr.translate.x;
          float y = tile.offset_y + tr.translate.y;                  

          // render tile
          draw_tile(tile.tileset, x, y, tile.row, tile.col, tilemap.tilesize, tile.flip);
        }
      }
    }

  private:
    FUSE_INLINE void draw_tile(asset_id tileset_id, float x, float y, int row, int col, int size, SDL_RendererFlip flip) {
      SDL_FRect dst_rect = { x * size, y * size, size, size};
      SDL_Rect src_rect = { row * size, col * size, size, size };
      auto& tileset = _assets->get<texture_asset>(tileset_id)->instance;
      SDL_RenderCopyExF(_renderer, tileset.data, &src_rect, &dst_rect, 0, NULL, flip);
      // render rectangle
      SDL_RenderDrawRectF(_renderer, &dst_rect);
    }
  };
}