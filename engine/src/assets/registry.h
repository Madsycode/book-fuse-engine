#pragma once

#include "font.h"
#include "audio.h"
#include "tilemap.h"
#include "texture.h"
#include "animation.h"
#include "core/typeid.h"

namespace fuse {
  struct asset_registry {
    FUSE_INLINE asset_registry() = default;

    FUSE_INLINE void clear() { 
      for(auto& [_, list] : _data){
        for(auto a : list) { FUSE_DELETE(a); }
      }
      _data.clear(); 
    }

    template <typename T>
    FUSE_INLINE T* get(asset_id id) {
      FUSE_STATIC_ASSERT(std::is_base_of<asset, T>::value);
      const auto type = typeid<T>();
      if(!_data.count(type)) { return NULL; }

      for(auto& asset : _data.at(type)) {
        if(asset->id == id) { return static_cast<T*>(asset); }
      }
      return NULL;
    }

     template <typename T>
    FUSE_INLINE T* get(const std::string& name) {
      asset_id id = get_id<T>(name);
      FUSE_STATIC_ASSERT(std::is_base_of<asset, T>::value);
      const auto type = type_id<T>();
      if(!_data.count(type)) { return NULL; }

      for(auto& asset : _data.at(type)) {
        if(asset->id == id) { return static_cast<T*>(asset); }
      }
      return NULL;
    }

    template<typename T>
    FUSE_INLINE asset_id get_id(const std::string& name) {
      FUSE_STATIC_ASSERT(std::is_base_of<asset, T>::value);
      const auto type = type_id<T>();
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
      _data[type_id<T>()].push_back(asset);
      return asset;
    }  

  private:
    std::unordered_map<asset_typeid, std::vector<asset*>> _data;
  };
}