#pragma once
#include "registry.h"
#include "helpers/yaml.h"

namespace fuse {
  struct asset_serializer {
    FUSE_INLINE asset_serializer(asset_registry* assets): _assets(assets) {}

    FUSE_INLINE void serialize(YAML::Emitter& em) {
      em << YAML::Key << "assets" << YAML::Value << YAML::BeginSeq;
      {
        // serialize animations
        serialize_animations(em);

        // serialize textures
        serialize_textures(em);

        // serialize audios
        serialize_audios(em);

        // serialize fonts
        serialize_fonts(em);;
      }
      em << YAML::EndSeq;
    }

    FUSE_INLINE void deserialize(YAML::Node& nodes, SDL_Renderer* r) {
      // clear registry
      _assets->clear();

      // deserialize assets
      for(auto node : nodes) {        
        deserialize_texture(node, r);
        deserialize_animation(node);
        deserialize_audio(node);
        deserialize_font(node);
      }          
    }

    // serialize texture_asset
    FUSE_INLINE void serialize_textures(YAML::Emitter& em) {
      for (auto asset : _assets->_data[type_id<texture_asset>()]) {
        auto texture = static_cast<texture_asset*>(asset);

        em << YAML::BeginMap;			
        {		
          em << YAML::Key << "texture";          
          em << YAML::BeginMap;
          {
            em << YAML::Key << "id" << YAML::Value << texture->id;
            em << YAML::Key << "name" << YAML::Value << texture->name;
            em << YAML::Key << "source" << YAML::Value << texture->instance.filename;
          }
          em << YAML::EndMap; 
        }
        em << YAML::EndMap;      
      }
    } 
    // deserialize texture_asset
    FUSE_INLINE void deserialize_texture(YAML::Node node, SDL_Renderer* target) {
      if (auto data = node["texture"]) {
        auto source = data["source"].as<std::string>();
        auto name = data["name"].as<std::string>();
        auto asset = _assets->load_texture(source, name, target);
        asset->id = data["id"].as<asset_id>();
      }
    }

    // serialize animation_asset
    FUSE_INLINE void serialize_animations(YAML::Emitter& em) {
      for (auto asset : _assets->_data[type_id<animation_asset>()]) {
        auto anim = static_cast<animation_asset*>(asset);
        
        em << YAML::BeginMap;			
        {		
          em << YAML::Key << "animation";
          em << YAML::BeginMap;
          {
            em << YAML::Key << "id" << YAML::Value << anim->id;
            em << YAML::Key << "name" << YAML::Value << anim->name;
            em << YAML::Key << "speed" << YAML::Value << anim->instance.speed;

            // serialize frames
            for(int i = 0; i < anim->instance.frames.size(); i++) {
              auto temp = "frame" + std::to_string(i);
              em << YAML::Key << temp << YAML::Value << anim->instance.frames[i];
            }
          }
          em << YAML::EndMap; 
        }
        em << YAML::EndMap;      
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

    // serialize audio_asset
    FUSE_INLINE void serialize_audios(YAML::Emitter& em) {
      for (auto asset : _assets->_data[type_id<audio_asset>()]) {
        auto audio = static_cast<audio_asset*>(asset);

        em << YAML::BeginMap;			
        {
          em << YAML::Key << "audio";
          em << YAML::BeginMap;
          {
            em << YAML::Key << "id" << YAML::Value << audio->id;
            em << YAML::Key << "name" << YAML::Value << audio->name;
            em << YAML::Key << "source" << YAML::Value << audio->instance.filename;
          }
          em << YAML::EndMap;
        }
        em << YAML::EndMap;
      }
    } 
    // deserialize audio_asset
    FUSE_INLINE void deserialize_audio(YAML::Node node) {
      if (auto data = node["audio"]) {
        auto name = data["name"].as<std::string>();
        auto source = data["source"].as<std::string>();
        auto asset = _assets->load_audio(source, name);
        asset->id = data["id"].as<asset_id>();
      }
    }

    // serialize font_asset
    FUSE_INLINE void serialize_fonts(YAML::Emitter& em) {
      for (auto asset : _assets->_data[type_id<font_asset>()]) {
        auto font = static_cast<font_asset*>(asset);

        em << YAML::BeginMap;			
        {
          em << YAML::Key << "font";          
          em << YAML::BeginMap;
          {
            em << YAML::Key << "id" << YAML::Value << font->id;
            em << YAML::Key << "name" << YAML::Value << font->name;
            em << YAML::Key << "size" << YAML::Value << font->instance.size;
            em << YAML::Key << "source" << YAML::Value << font->instance.filename;        
          }
          em << YAML::EndMap;
        }
        em << YAML::EndMap;
      }
    }   
    // deserialize font_asset
    FUSE_INLINE void deserialize_font(YAML::Node node) {
      if (auto data = node["font"]) {
        auto size = data["size"].as<int>();
        auto name = data["name"].as<std::string>();
        auto source = data["source"].as<std::string>();
        auto asset = _assets->load_font(source, name, size);
        asset->id = data["id"].as<asset_id>();
      }
    }

  private:
    asset_registry* _assets = NULL;
  };
}
