#pragma once
#include "entity.h"
#include "helpers/yaml.h"
#include "components/common.h"
#include "components/physics.h"
#include "components/graphics.h"
#include "components/behaviour.h"

namespace fuse::ecs {
  struct serializer {
    FUSE_INLINE serializer(registry* reg): _registry(reg) {}

    FUSE_INLINE void deserialize(YAML::Node nodes) {      
      // clear registry
      _registry->clear();

      // deserialize entities
      for(auto node : nodes) {
        ecs::entity entity(_registry);          
        deserialize_info(node, entity);
        deserialize_transform(node, entity);
        deserialize_rigidbody(node, entity);
        deserialize_collider(node, entity);
        deserialize_sprite(node, entity);
        deserialize_animation(node, entity);
        deserialize_audio(node, entity);
        deserialize_text(node, entity);
        deserialize_script(node, entity);
      }  
    }

    FUSE_INLINE void serialize(YAML::Emitter& emitter) {      
      emitter << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
      {
        for (auto& e : _registry->view<ecs::info_component>()) {
          ecs::entity entity(e, _registry);
          emitter << YAML::BeginMap;
          {
            serialize_info(emitter, entity);
            serialize_transform(emitter, entity);
            serialize_rigidbody(emitter, entity);
            serialize_collider(emitter, entity);
            serialize_sprite(emitter, entity);
            serialize_animation(emitter, entity);
            serialize_audio(emitter, entity);
            serialize_text(emitter, entity);
            serialize_script(emitter, entity);
          }
          emitter << YAML::EndMap;
        }
      }
      emitter << YAML::EndSeq;
    }

    // ++
    
    // serialize info_component
    FUSE_INLINE void serialize_info(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::info_component>()) {
        auto& i = e.get_component<ecs::info_component>();
        em << YAML::Key << "info_component";
				em << YAML::BeginMap;
				em << YAML::Key << "uuid" << YAML::Value << i.uuid;
				em << YAML::Key << "name" << YAML::Value << i.name;
				em << YAML::Key << "tag" << YAML::Value << i.tag;
				em << YAML::EndMap;
			}
		}

    // serialize transform_component
    FUSE_INLINE void serialize_transform(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::transform_component>()) {
        auto& t = e.get_component<ecs::transform_component>().transform;
        em << YAML::Key << "transform_component";
				em << YAML::BeginMap;
				em << YAML::Key << "translate" << YAML::Value << t.translate;
				em << YAML::Key << "rotation" << YAML::Value << t.rotation;
				em << YAML::Key << "scale" << YAML::Value << t.scale;
				em << YAML::EndMap;
			}
		}

    // serialize rigidbody_component
    FUSE_INLINE void serialize_rigidbody(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::rigidbody_component>()) {
        auto& r = e.get_component<ecs::rigidbody_component>();
        em << YAML::Key << "rigidbody_component";
				em << YAML::BeginMap;
				em << YAML::Key << "gravity" << YAML::Value << r.body.gravity_scale;
				em << YAML::Key << "velocity" << YAML::Value << r.body.velocity;
				em << YAML::Key << "disabled" << YAML::Value << r.disabled;
				em << YAML::Key << "force" << YAML::Value << r.body.force;
				em << YAML::EndMap;
			}
		}

    // serialize collider_component
    FUSE_INLINE void serialize_collider(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::collider_component>()) {
        auto& c = e.get_component<ecs::collider_component>();
        em << YAML::Key << "collider_component";
				em << YAML::BeginMap;
				em << YAML::Key << "width" << YAML::Value << c.collider.w;
				em << YAML::Key << "height" << YAML::Value << c.collider.h;
				em << YAML::Key << "disabled" << YAML::Value << c.disabled;
				em << YAML::EndMap;
			}
		}

    // serialize sprite_component
    FUSE_INLINE void serialize_sprite(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::sprite_component>()) {
        auto& s = e.get_component<ecs::sprite_component>();
        em << YAML::Key << "sprite_component";
				em << YAML::BeginMap;
				em << YAML::Key << "sprite" << YAML::Value << s.sprite;
				em << YAML::Key << "flip" << YAML::Value << s.flip;
				em << YAML::EndMap;
			}
		}

    // serialize audio_component
    FUSE_INLINE void serialize_audio(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::audio_component>()) {
        auto& a = e.get_component<ecs::audio_component>();
        em << YAML::Key << "audio_component";
				em << YAML::BeginMap;
				em << YAML::Key << "audio" << YAML::Value << a.audio;
				em << YAML::EndMap;
			}
		}

    // serialize animation_component
    FUSE_INLINE void serialize_animation(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::animation_component>()) {
        auto& a = e.get_component<ecs::animation_component>();
        em << YAML::Key << "animation_component";
				em << YAML::BeginMap;
				em << YAML::Key << "animation" << YAML::Value << a.animation;
				em << YAML::Key << "flip" << YAML::Value << (int)a.flip;
				em << YAML::EndMap;
			}
		}

    // serialize text_component
    FUSE_INLINE void serialize_text(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::text_component>()) {
        auto& t = e.get_component<ecs::text_component>();
        em << YAML::Key << "text_component";
				em << YAML::BeginMap;
				em << YAML::Key << "R" << YAML::Value << (int)t.color.r;
				em << YAML::Key << "G" << YAML::Value << (int)t.color.g;
				em << YAML::Key << "B" << YAML::Value << (int)t.color.b;
				em << YAML::Key << "A" << YAML::Value << (int)t.color.a;
				em << YAML::Key << "text" << YAML::Value << t.text;
				em << YAML::Key << "font" << YAML::Value << t.font;
				em << YAML::Key << "flip" << YAML::Value << t.flip;
				em << YAML::EndMap;
			}
		}

    // serialize script_component
    FUSE_INLINE void serialize_script(YAML::Emitter& em, ecs::entity& e) {
			if (e.has_component<ecs::script_component>()) {
        auto& s = e.get_component<ecs::script_component>();
        em << YAML::Key << "script_component";
				em << YAML::BeginMap;
				em << YAML::Key << "name" << YAML::Value << s.name;
				em << YAML::EndMap;
			}
		}

    // ++

    // deserialize info_component
    FUSE_INLINE void deserialize_info(YAML::Node node, ecs::entity& e) {
			if (auto data = node["info_component"]) {
        auto& i = e.add_component<ecs::info_component>();
        i.name = data["name"].as<std::string>();
        i.tag = data["tag"].as<std::string>();
        i.uuid = data["uuid"].as<uuid64>();
			}
		}

    // deserialize transform_component
    FUSE_INLINE void deserialize_transform(YAML::Node node, ecs::entity& e) {
			if (auto data = node["transform_component"]) {
        auto& t = e.add_component<ecs::transform_component>();
        t.transform.translate = data["translate"].as<vec2f>();
        t.transform.rotation = data["rotation"].as<float>();
        t.transform.scale = data["scale"].as<vec2f>();
			}
		}

    // deserialize rigidbody_component
    FUSE_INLINE void deserialize_rigidbody(YAML::Node node, ecs::entity& e) {
			if (auto data = node["rigidbody_component"]) {
        auto& r = e.add_component<ecs::rigidbody_component>();
        r.body.gravity_scale = data["gravity"].as<float>();
        r.body.velocity = data["velocity"].as<vec2f>();
        r.body.force = data["force"].as<vec2f>();
        r.disabled = data["disabled"].as<bool>();
			}
		}

    // deserialize collider_component
    FUSE_INLINE void deserialize_collider(YAML::Node node, ecs::entity& e) {
			if (auto data = node["collider_component"]) {
        auto& c = e.add_component<ecs::collider_component>();
        c.collider.w = data["width"].as<float>();
        c.collider.h = data["height"].as<float>();
        c.disabled = data["disabled"].as<bool>();
			}
		}

    // deserialize sprite_component
    FUSE_INLINE void deserialize_sprite(YAML::Node node, ecs::entity& e) {
			if (auto data = node["sprite_component"]) {
        auto& s = e.add_component<ecs::sprite_component>();
        s.flip = (SDL_RendererFlip)data["flip"].as<int>();
        s.sprite = data["sprite"].as<asset_id>();
			}
		}

    // deserialize audio_component
    FUSE_INLINE void deserialize_audio(YAML::Node node, ecs::entity& e) {
			if (auto data = node["audio_component"]) {
        auto& a = e.add_component<ecs::audio_component>();
        a.audio = data["audio"].as<asset_id>();      
			}
		}
    
    // deserialize animation_component
    FUSE_INLINE void deserialize_animation(YAML::Node node, ecs::entity& e) {
			if (auto data = node["animation_component"]) {
        auto& a = e.add_component<ecs::animation_component>();
        a.flip = (SDL_RendererFlip)data["flip"].as<int>();
        a.animation = data["animation"].as<asset_id>();
			}
		}

    // deserialize text_component
    FUSE_INLINE void deserialize_text(YAML::Node node, ecs::entity& e) {
			if (auto data = node["text_component"]) {
        auto& t = e.add_component<ecs::text_component>();
        t.flip = (SDL_RendererFlip)data["flip"].as<int>();
        t.text = data["text"].as<std::string>();        
        t.font = data["font"].as<asset_id>();
        t.color.r = data["R"].as<int>();
        t.color.g = data["G"].as<int>();
        t.color.b = data["B"].as<int>();
        t.color.a = data["A"].as<int>();
			}
		}

    // deserialize script_component
    FUSE_INLINE void deserialize_script(YAML::Node node, ecs::entity& e) {
			if (auto data = node["script_component"]) {
        auto& s = e.add_component<ecs::script_component>();
        s.name = data["name"].as<std::string>();

        if(!s.name.compare("player_controller")) {
          s.bind<player_controller>();
        }
        else if(!s.name.compare("scrolling_ground")) {
          s.bind<scrolling_ground>();
        }
        else if(!s.name.compare("game_controller")) {
          s.bind<game_controller>();
        }
  			}
		}

  private:
    registry* _registry = NULL;
  };
}