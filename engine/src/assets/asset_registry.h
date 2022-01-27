#pragma once

#include "font.h"
#include "audio.h"
#include "tilemap.h"
#include "texture.h"
#include "animation.h"
#include "core/typeid.h"

namespace fuse {
  struct asset_registry {
    FUSE_INLINE asset_registry() {
      // init SDL_image
      if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP | IMG_INIT_TIF) < 0) {
        FUSE_ERROR("IMG_Init failed %s", IMG_GetError());
      }

      // init SDL_ttf
      if(TTF_Init() < 0) { 
        FUSE_ERROR("TTF_Init failed: %s", TTF_GetError());         
      }

      // init SDL_mixer
      if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OPUS | MIX_INIT_OGG) < 0) {
        FUSE_ERROR("Mix_Init failed: %s", Mix_GetError());
      }
      if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0) {
        FUSE_ERROR("Mix_OpenAudio failed: %s", Mix_GetError());
      }
    }

    FUSE_INLINE void clear() { 
      for(auto& [_, list] : _data){
        for(auto a : list) { FUSE_DELETE(a); }
      }
      _data.clear(); 
    }

    template <typename T>
    FUSE_INLINE T* get(asset_id id) {
      FUSE_STATIC_ASSERT(std::is_base_of<asset_instance, T>::value);
      const type_id type = get_typeid<T>();
      if(!_data.count(type)) { return NULL; }

      for(auto& asset : _data.at(type)) {
        if(asset->id == id) { return static_cast<T*>(asset); }
      }
      return NULL;
    }

     template <typename T>
    FUSE_INLINE T* get(const std::string& name) {
      asset_id id = get_id<T>(name);
      FUSE_STATIC_ASSERT(std::is_base_of<asset_instance, T>::value);
      const type_id type = get_typeid<T>();
      if(!_data.count(type)) { return NULL; }

      for(auto& asset : _data.at(type)) {
        if(asset->id == id) { return static_cast<T*>(asset); }
      }
      return NULL;
    }

    template<typename T>
    FUSE_INLINE asset_id get_id(const std::string& name) {
      FUSE_STATIC_ASSERT(std::is_base_of<asset_instance, T>::value);
      type_id type = get_typeid<T>();
      if(!_data.count(type)) { return INVALID_ID; }

      for(auto& asset : _data.at(type)) {
        if(!asset->name.compare(name)) { return asset->id; }
      }
      return INVALID_ID;
    }  

    template <typename T>
    FUSE_INLINE T* add(const std::string& name) {
      auto asset = new T();
      asset->name = name;
      _data[get_typeid<T>()].push_back(asset);
      return asset;
    }  

    FUSE_INLINE audio_asset* import_audio(const std::string& path, const std::string& name) {
        audio_instance audio;
        audio.data = Mix_LoadWAV(path.c_str());
        audio.filename = path;

        if (!audio.data) {
          FUSE_ERROR("%s", IMG_GetError());
          return NULL;
        }

        auto asset = new audio_asset(name, audio);
        _data[get_typeid<audio_asset>()].push_back(asset);
        return asset;
      }

    FUSE_INLINE font_asset* import_font(const std::string& path, const std::string& name, int size) {
      font_instance font;
      font.data = TTF_OpenFont(path.c_str(), size);
      font.filename = path;
      font.size = size;

      if (!font.data) {
        FUSE_ERROR("%s", IMG_GetError());
        return NULL;
      }

      auto asset = new font_asset(name, font);
      _data[get_typeid<font_asset>()].push_back(asset);
      return asset;
    }

    FUSE_INLINE texture_asset* import_texture(const std::string& path, const std::string& name, SDL_Renderer* target) {
      texture_instance tx;
      tx.data = IMG_LoadTexture(target, path.c_str());
      tx.filename = path;

      if (!tx.data) {
        FUSE_ERROR("%s", IMG_GetError());
        return NULL;
      }      
      
      SDL_QueryTexture(tx.data, NULL, NULL, &tx.width, &tx.height);
      auto asset = new texture_asset(name, tx);
      _data[get_typeid<texture_asset>()].push_back(asset);
      return asset;
    }

  private:
    friend struct asset_serializer;
    std::unordered_map<type_id, asset_list> _data;
  };
}