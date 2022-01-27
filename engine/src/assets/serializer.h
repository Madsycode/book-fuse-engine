#pragma once
#include "registry.h"
#include "helpers/yaml.h"

namespace fuse {
  struct asset_serializer {
    FUSE_INLINE asset_serializer(asset_registry* assets): _assets(assets) {}

    FUSE_INLINE void deserialize(YAML::Node& nodes, SDL_Renderer* rd) {
      // clear registry
      _assets->clear();

      // deserialize assets
      for(auto node : nodes) {        
        deserialize_texture(node, rd);
        deserialize_animation(node);
        deserialize_audio(node);
        deserialize_font(node);
      }          
    }

    FUSE_INLINE void serialize(YAML::Emitter& emitter) {
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
    }

    // ++

    // serialize animation_asset
    FUSE_INLINE void serialize_animation(YAML::Emitter& em, animation_asset* asset) {
			if (asset) {
        em << YAML::Key << "animation";
				em << YAML::BeginMap;
				em << YAML::Key << "id" << YAML::Value << asset->id;
				em << YAML::Key << "name" << YAML::Value << asset->name;
				em << YAML::Key << "speed" << YAML::Value << asset->instance.speed;

        // serialize frames
        for(int i = 0; i < asset->instance.frames.size(); i++) {
          auto temp = "frame" + std::to_string(i);
          em << YAML::Key << temp << YAML::Value << asset->instance.frames[i];
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
				em << YAML::Key << "source" << YAML::Value << asset->instance.filename;
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
				em << YAML::Key << "source" << YAML::Value << asset->instance.filename;
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
				em << YAML::Key << "size" << YAML::Value << asset->instance.size;
				em << YAML::Key << "source" << YAML::Value << asset->instance.filename;        
				em << YAML::EndMap;
			}
		}   

    // ++

    // deserialize texture_asset
    FUSE_INLINE void deserialize_texture(YAML::Node node, SDL_Renderer* target) {
			if (auto data = node["texture"]) {
        auto source = data["source"].as<std::string>();
        auto name = data["name"].as<std::string>();
        auto asset = _assets->import_texture(source, name, target);
        asset->id = data["id"].as<asset_id>();
			}
		}

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
          asset->instance.frames.push_back(frame);                  
          i++;
        }
        asset->instance.speed = data["speed"].as<int>();        
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