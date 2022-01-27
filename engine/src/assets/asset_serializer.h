#pragma once
#include "helpers/yaml.h"
#include "asset_registry.h"

namespace fuse {
  struct asset_serializer {
    FUSE_INLINE asset_serializer(asset_registry* assets): _assets(assets) {}

    FUSE_INLINE bool deserialize(YAML::Node& asset_nodes, SDL_Renderer* target) {
      _assets->clear();  
      YAML::Node nodes = asset_nodes;
      for(auto node : nodes) {        
        deserialize_texture(node, target);
        deserialize_animation(node);
        deserialize_audio(node);
        deserialize_font(node);
      }          
      return true;
    }

    FUSE_INLINE bool serialize(YAML::Emitter& emitter) {
      emitter << YAML::Key << "assets" << YAML::Value << YAML::BeginSeq;
      {
        // serialize animation
        for (auto asset : _assets->_data[get_typeid<animation_asset>()]) {
          emitter << YAML::BeginMap;						
          serialize_animation(emitter, static_cast<animation_asset*>(asset));						
          emitter << YAML::EndMap;
        }
        // serialize textures
        for (auto asset : _assets->_data[get_typeid<texture_asset>()]) {
          emitter << YAML::BeginMap;						
          serialize_texture(emitter, static_cast<texture_asset*>(asset));						
          emitter << YAML::EndMap;
        }
        // serialize audios
        for (auto asset : _assets->_data[get_typeid<audio_asset>()]) {
          emitter << YAML::BeginMap;						
          serialize_audio(emitter, static_cast<audio_asset*>(asset));						
          emitter << YAML::EndMap;
        }
        // serialize fonts
        for (auto asset : _assets->_data[get_typeid<font_asset>()]) {
          emitter << YAML::BeginMap;						
          serialize_font(emitter, static_cast<font_asset*>(asset));						
          emitter << YAML::EndMap;
        }
      }
      emitter << YAML::EndSeq;
      return true;
    }

    // ++
    // serialize animation_asset
    FUSE_INLINE void serialize_animation(YAML::Emitter& em, animation_asset* asset) {
			if (asset) {
        em << YAML::Key << "animation";
				em << YAML::BeginMap;
				em << YAML::Key << "id" << YAML::Value << asset->id;
				em << YAML::Key << "name" << YAML::Value << asset->name;
				em << YAML::Key << "speed" << YAML::Value << asset->animation.speed;

        // serialize frames
        for(int i = 0; i < asset->animation.frames.size(); i++) {
          auto temp = "frame" + std::to_string(i);
          em << YAML::Key << temp << YAML::Value << asset->animation.frames[i];
        }
				em << YAML::EndMap;
			}
		}   

    // serialize texture_asset
    FUSE_INLINE void serialize_texture(YAML::Emitter& em, texture_asset* asset) {
			if (asset) {
        em << YAML::Key << "texture";
				em << YAML::BeginMap;
				em << YAML::Key << "id" << YAML::Value << asset->id;
				em << YAML::Key << "name" << YAML::Value << asset->name;
				em << YAML::Key << "source" << YAML::Value << asset->texture.filename;
				em << YAML::EndMap;
			}
		} 

    // serialize audio_asset
    FUSE_INLINE void serialize_audio(YAML::Emitter& em, audio_asset* asset) {
			if (asset) {
        em << YAML::Key << "audio";
				em << YAML::BeginMap;
				em << YAML::Key << "id" << YAML::Value << asset->id;
				em << YAML::Key << "name" << YAML::Value << asset->name;
				em << YAML::Key << "source" << YAML::Value << asset->audio.filename;
				em << YAML::EndMap;
			}
		} 

    // serialize font_asset
    FUSE_INLINE void serialize_font(YAML::Emitter& em, font_asset* asset) {
			if (asset) {
        em << YAML::Key << "font";
				em << YAML::BeginMap;
				em << YAML::Key << "id" << YAML::Value << asset->id;
				em << YAML::Key << "name" << YAML::Value << asset->name;
				em << YAML::Key << "size" << YAML::Value << asset->font.size;
				em << YAML::Key << "source" << YAML::Value << asset->font.filename;        
				em << YAML::EndMap;
			}
		}   

    // ++

    // deserialize animation_asset
    FUSE_INLINE void deserialize_animation(YAML::Node node) {
			if (const auto data = node["animation"]) {
        auto name = data["name"].as<std::string>();
        auto asset = _assets->add<animation_asset>(name);

        // deserialize frames
        int i = 0;
        while(true) {
          auto temp = "frame" + std::to_string(i);
          if(!data[temp]) { break; }

          auto frame = data[temp].as<asset_id>();
          asset->animation.frames.push_back(frame);                  
          i++;
        }
        asset->animation.speed = data["speed"].as<int>();        
        asset->id = data["id"].as<asset_id>();
			}
		}

    // deserialize texture_asset
    FUSE_INLINE void deserialize_texture(YAML::Node node, SDL_Renderer* target) {
			if (auto data = node["texture"]) {
        auto source = data["source"].as<std::string>();
        auto name = data["name"].as<std::string>();
        auto asset = _assets->import_texture(source, name, target);
        asset->id = data["id"].as<asset_id>();
			}
		}

    // deserialize audio_asset
    FUSE_INLINE void deserialize_audio(YAML::Node node) {
			if (auto data = node["audio"]) {
        auto name = data["name"].as<std::string>();
        auto source = data["source"].as<std::string>();
        auto asset = _assets->import_audio(source, name);
        asset->id = data["id"].as<asset_id>();
			}
		}

    // deserialize font_asset
    FUSE_INLINE void deserialize_font(YAML::Node node) {
			if (auto data = node["font"]) {
        auto size = data["size"].as<int>();
        auto name = data["name"].as<std::string>();
        auto source = data["source"].as<std::string>();
        auto asset = _assets->import_font(source, name, size);
        asset->id = data["id"].as<asset_id>();
			}
		}

  private:
    asset_registry* _assets = NULL;
  };
}