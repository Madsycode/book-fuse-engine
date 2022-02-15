#pragma once
#include "ecs/components/graphics.h"
#include "ecs/components/physics.h"
#include "ecs/components/common.h"
#include "ecs/entity.h"

#include "assets/registry.h"
#include "core/inputs.h"
#include "math/random.h"

namespace fuse {
  struct script_props {
    FUSE_INLINE script_props(ecs::entityid e, ecs::registry* r, asset_registry* a):
    entity(e), registry(r), assets(a) {}
    ecs::entityid entity = INVALID_ID;
    ecs::registry* registry = NULL;
    asset_registry* assets = NULL;
  };

  struct script_instance {
    FUSE_INLINE virtual ~script_instance() = default;

    FUSE_INLINE void init(const script_props& props) {
      this->_registry = props.registry;
      this->_assets = props.assets;
      this->_entity = props.entity;
    }

    FUSE_INLINE void virtual on_collision(ecs::entity) {}
    FUSE_INLINE void virtual on_update(float) {}
    FUSE_INLINE void virtual on_destroy() {}
    FUSE_INLINE void virtual on_start() {}

  protected:
    template<typename T, typename... Args>
    FUSE_INLINE T& add_component(Args&&... args) {
      return _registry->add_component<T>(_entity, std::forward<Args>(args)...);
    }

    template<typename T>
    FUSE_INLINE T& get_component() {
      return _registry->get_component<T>(_entity);
    }

    template<typename T>
    FUSE_INLINE void remove_component() {
      return _registry->remove_component<T>(_entity);
    }

    template<typename T>
    FUSE_INLINE bool has_component() {
      return _registry->has_component<T>(_entity);
    }

    FUSE_INLINE ecs::entity add_entity() {
      auto entity = _registry->add_entity();
      _registry->add_component<ecs::transform_component>(entity);
      _registry->add_component<ecs::info_component>(entity);
      return ecs::entity(entity, _registry);
    }

    FUSE_INLINE void destroy() {
      _registry->destroy(_entity);
    }

    FUSE_INLINE ecs::entity find_entity(const std::string& name) {
      for(auto& e : _registry->view<ecs::info_component>()) {
        auto& i = _registry->get_component<ecs::info_component>(e);
        
        if(!i.name.compare(name)) { 
          return ecs::entity(e, _registry); 
        }
      }
      return ecs::entity();
    }

    template<typename T>
    FUSE_INLINE T* get_asset(const std::string& name) {
      return _assets->get<T>(name);
    }

FUSE_INLINE void play_audio(std::string name, int channel = -1, int loops = 0, int vol = 50) {
  auto& chunck = _assets->get<audio_asset>(name)->instance.data;
  Mix_VolumeChunk(chunck, vol);                
  Mix_PlayChannel(channel, chunck, loops);     
}

  private:
    ecs::entityid _entity = INVALID_ID;
    ecs::registry* _registry = NULL;
    asset_registry* _assets = NULL;
  };

  // script allocator function type
  using script_instantiator = std::function<script_instance*(const script_props&)>;
} 
