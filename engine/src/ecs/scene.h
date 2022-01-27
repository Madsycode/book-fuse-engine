#pragma once
#include "systems/tilemap_renderer_system.h"
#include "systems/sprite_renderer_system.h"
#include "systems/text_renderer_system.h"
#include "systems/animation_system.h"
#include "systems/rigidbody_system.h"
#include "systems/collision_system.h"
#include "systems/script_system.h"

#include "scripts/player_controller.h"
#include "scripts/scrolling_ground.h"
#include "scripts/game_controller.h"

#include "assets/serializer.h"
#include "ecs/serializer.h"

namespace fuse::ecs {
  struct scene {
    FUSE_INLINE scene(SDL_Renderer* target): _renderer(target) {
      register_system<ecs::script_system>();
      register_system<ecs::rigidbody_system>();
      register_system<ecs::collision_system>();
      register_system<ecs::tilemap_renderer_system>();
      register_system<ecs::sprite_renderer_system>();
      register_system<ecs::animation_system>();
      register_system<ecs::text_renderer_system>();
    }

    FUSE_INLINE ~scene() {
      for (auto sys : _systems) { FUSE_DELETE(sys); }
      _registry.clear();
      _assets.clear();
      _systems.clear();
    }

    FUSE_INLINE ecs::entity add_entity(const std::string& name) {
      auto entity = ecs::entity(&_registry);
      entity.add_component<info_component>().name = name;
      entity.add_component<transform_component>();
      return entity;
    }

    FUSE_INLINE void update(float dt) {
      // set renderer clear color
      SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);      

      // update registered systems
      for (auto& sys : _systems) { sys->update(dt); }

      // render colliders
      SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
      for(auto& e : _registry.view<collider_component>()) {
        auto& cl = _registry.get_component<collider_component>(e);
        SDL_RenderDrawRectF(_renderer, &cl.collider);
      }
    }

    FUSE_INLINE void create_entities() {
      auto screen_size = inputs::display_size();

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
      fly_a->instance.frames.push_back(fly1->id);                       
      fly_a->instance.frames.push_back(fly2->id);                       

      // add player hurt animation
      auto hurt_a = _assets.add<animation_asset>("hurt");
      hurt_a->instance.frames.push_back(hurt->id);  

      // add player entity
      auto player = add_entity("player");
      auto& ps = player.add_component<ecs::script_component>();
      ps.bind<player_controller>();
      ps.name = "player_controller";
      player.add_component<ecs::audio_component>().audio = boom->id;
      auto& tr = player.get_component<transform_component>();
      tr.transform.translate = vec2f(screen_size.x/3, screen_size.y/2);
      tr.transform.scale = vec2f(0.15f);
      auto& rb = player.add_component<rigidbody_component>();
      rb.body.gravity_scale = 25.0f;
      auto& an = player.add_component<animation_component>();
      an.animation = fly_a->id;
      auto& cl = player.add_component<collider_component>();
      cl.collider = { 0, 0, 58, 38 };

      // background 1            
      auto background = add_entity("background");
      auto& bg_tr = background.add_component<transform_component>();
      bg_tr.transform.translate = vec2f(-5, -5);
      auto& bg_sp = background.add_component<sprite_component>(); 
      bg_sp.sprite = bg->id;

      // ground
      auto ground = add_entity("ground");
      auto& gds = ground.add_component<ecs::script_component>();
      gds.bind<scrolling_ground>();
      gds.name = "scrolling_ground";
      auto& gd_tr = ground.get_component<transform_component>();
      gd_tr.transform.translate = vec2f(0.0f, screen_size.y - 100);
      auto& gd_rb = ground.add_component<rigidbody_component>();
      gd_rb.body.velocity.x = -100.0f;
      auto& gd_sp = ground.add_component<sprite_component>(); 
      gd_sp.sprite = gd->id;
      auto& gd_cl =ground.add_component<collider_component>();
      gd_cl.collider = { 0, 0, (float)gd->instance.width, (float)gd->instance.height };
      
      // score text entity
      auto score = add_entity("score");     
      auto& sc_tr = score.get_component<transform_component>();
      sc_tr.transform.translate = vec2f(120, 20);
      auto& tx = score.add_component<text_component>();
      tx.text = "Score: 0";
      tx.font = font->id;

      // game spawner
      auto game = add_entity("game");
      auto& gs = game.add_component<script_component>();
      gs.bind<game_controller>();
      gs.name = "game_controller";
      game.add_component<audio_component>().audio = music->id;

      // start systems
      for (auto& sys : _systems) { sys->start(); }
    }

    template <typename T>
    FUSE_INLINE void register_system() {
      FUSE_STATIC_ASSERT(std::is_base_of<ecs::system, T>::value);
      auto system = new T();
      system->prepare(&_registry, _renderer, &_assets);
      this->_systems.push_back(system);
    }

    FUSE_INLINE bool serialize(const std::string& path) {
      YAML::Emitter emitter;
      emitter << YAML::BeginMap;
      {
        // serialize assets
        asset_serializer(&_assets).serialize(emitter);
        // serialize entities
        ecs::serializer(&_registry).serialize(emitter);
      }
      emitter << YAML::EndMap;

      std::ofstream filepath(path);
			filepath << emitter.c_str();
      return true;
    }

    FUSE_INLINE bool deserialize(const std::string& path) {
      YAML::Node root;
      try {
        root = YAML::LoadFile(path);
      }
      catch (YAML::ParserException e) {
        FUSE_INFO("failed to deserialize scene!");
        return false;
      }  

      // deserialize assets
      if(auto nodes = root["assets"]) {
        asset_serializer(&_assets).deserialize(nodes, _renderer);
      }

      // deserialize entities
      if(auto nodes = root["entities"]) {
        ecs::serializer(&_registry).deserialize(nodes);
      }

      // start scene
      for (auto& sys : _systems) { sys->start(); }
    }

    private:
      friend struct scene_serializer;
      std::vector<system*> _systems;
      SDL_Renderer* _renderer = NULL;
      asset_registry _assets;
      registry _registry;
  };
}