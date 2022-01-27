#pragma once
#include "ecs/systems/tilemap_renderer_system.h"
#include "ecs/systems/sprite_renderer_system.h"
#include "ecs/systems/text_renderer_system.h"
#include "ecs/systems/animation_system.h"
#include "ecs/systems/rigidbody_system.h"
#include "ecs/systems/collision_system.h"
#include "ecs/systems/script_system.h"

#include "scripts/player_controller.h"
#include "scripts/scrolling_ground.h"
#include "scripts/game_controller.h"

namespace fuse {
  struct scene_instance {
    FUSE_INLINE scene_instance(SDL_Renderer *rd, dispatcher *dp): _renderer(rd), _dispatcher(dp) {
      register_system<ecs::script_system>();
      register_system<ecs::rigidbody_system>();
      register_system<ecs::collision_system>();
      register_system<ecs::tilemap_renderer_system>();
      register_system<ecs::sprite_renderer_system>();
      register_system<ecs::animation_system>();
      register_system<ecs::text_renderer_system>();
    }

    FUSE_INLINE ~scene_instance() {
      for (auto sys : _systems) { FUSE_DELETE(sys); }
      _registry.clear();
      _assets.clear();
      _systems.clear();
    }

    FUSE_INLINE ecs::entity add_entity(const std::string& name) {
      auto entity = ecs::entity(&_registry);
      entity.add_component<ecs::info_component>().name = name;
      entity.add_component<ecs::transform_component>();
      return entity;
    }

    FUSE_INLINE void update(float dt) {
      // set renderer clear color
      SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);      

      // update registered systems
      for (auto& sys : _systems) { sys->update(dt); }

      // render colliders
      SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
      for(auto& e : _registry.view<ecs::collider_component>()) {
        auto& cl = _registry.get_component<ecs::collider_component>(e);
        SDL_RenderDrawRectF(_renderer, &cl.collider);
      }
    }

    FUSE_INLINE void start() {
      for (auto& sys : _systems) { sys->start(); }
    }

    FUSE_INLINE void start() {
      // load player sprites
      auto fly1 = _assets.import_texture("assets/fly_1.png", "fly_1", _renderer);
      auto fly2 = _assets.import_texture("assets/fly_2.png", "fly_2", _renderer);
      auto hurt = _assets.import_texture("assets/hurt_1.png", "hurt", _renderer);
      // load pipe sprite
      auto pipe = _assets.import_texture("assets/pipe.png", "pipe", _renderer);
      // load bg texture
      auto gd = _assets.import_texture("assets/ground.png", "ground", _renderer);
      auto bg = _assets.import_texture("assets/bg1.png", "bg", _renderer);
      // load text font 
      auto font = _assets.import_font("assets/font.ttf", "font", 30);
      // load sound effects
      auto music = _assets.import_audio("assets/song.mp3", "music");
      auto boom = _assets.import_audio("assets/boom.wav", "boom");

      // add player fly animation
      auto fly_a = _assets.add<animation_asset>("fly");
      fly_a->animation.frames.push_back(fly1->id);                       
      fly_a->animation.frames.push_back(fly2->id);                       

      // add player hurt animation
      auto hurt_a = _assets.add<animation_asset>("hurt");
      hurt_a->animation.frames.push_back(hurt->id);  

      // add player entity
      auto player = add_entity("player");
      player.add_component<ecs::script_component>().bind<player_controller>();
      player.add_component<ecs::audio_component>().audio = boom->id;
      auto& tr = player.get_component<ecs::transform_component>();
      tr.transform.translate = vec2f(SCREEN_WIDTH/3, SCREEN_HEIGHT/2);
      tr.transform.scale = vec2f(0.15f);
      auto& rb = player.add_component<ecs::rigidbody_component>();
      rb.body.gravity_scale = 25.0f;
      auto& an = player.add_component<ecs::animation_component>();
      an.animation = fly_a->id;
      auto& cl = player.add_component<ecs::collider_component>();
      cl.collider = { 0, 0, 58, 38 };

      // background 1            
      auto background = add_entity("background");
      auto& bg_tr = background.add_component<ecs::transform_component>();
      bg_tr.transform.translate = vec2f(-5, -5);
      auto& bg_sp = background.add_component<ecs::sprite_component>(); 
      bg_sp.sprite = bg->id;

      // ground
      auto ground = add_entity("ground");
      ground.add_component<ecs::script_component>().bind<scrolling_ground>();
      auto& gd_tr = ground.get_component<ecs::transform_component>();
      gd_tr.transform.translate = vec2f(0.0f, SCREEN_HEIGHT - 100);
      auto& gd_rb = ground.add_component<ecs::rigidbody_component>();
      gd_rb.body.velocity.x = -100.0f;
      auto& gd_sp = ground.add_component<ecs::sprite_component>(); 
      gd_sp.sprite = gd->id;
      auto& gd_cl =ground.add_component<ecs::collider_component>();
      gd_cl.collider = { 0, 0, (float)gd->texture.width, (float)gd->texture.height };
      
      // score text entity
      auto score = add_entity("score");     
      auto& sc_tr = score.get_component<ecs::transform_component>();
      sc_tr.transform.translate = vec2f(120, 20);
      auto& tx = score.add_component<ecs::text_component>();
      tx.text = "Score: 0";
      tx.font = font->id;

      // game spawner
      auto game = add_entity("game");
      game.add_component<ecs::script_component>().bind<game_controller>();
      game.add_component<ecs::audio_component>().audio = music->id;

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
      friend struct scene_serializer;
      dispatcher* _dispatcher = NULL;
      SDL_Renderer* _renderer = NULL;
      ecs::system_list _systems;
      ecs::registry _registry;
      asset_registry _assets;
  };
}