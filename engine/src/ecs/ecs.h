#pragma once

#include "core/typeid.h"

namespace fuse::ecs {
  	
  // type definitions
	using entity_id = unsigned int; 
	using signature = std::set<type_id>; 
	using entity_list = std::set<entity_id>; 

  // component array data-entry
	template<typename T>
	struct component_entry {
		FUSE_INLINE component_entry(entity_id e, const T& d): entity(e), data(d) {}
		FUSE_INLINE operator entity_id() const { return entity; }
		entity_id entity = INVALID_ID;
		T data;
	};

  // component array interface
  struct array_instance {
		FUSE_INLINE virtual ~array_instance() = default;
		virtual void erase(entity_id) = 0;
	};

  // component array
	template<typename T>
	struct component_array : array_instance {				
		FUSE_INLINE T& push(entity_id entity, const T& data) {
			if (!this->exits(entity)) {
				_data.push_back(component_entry(entity, data));
				return _data.back().data;
			}
			return get(entity);
		}	

		FUSE_INLINE void erase(entity_id entity) {	
			auto it = std::find(_data.begin(), _data.end(), entity);
			if (it != _data.end()) { _data.erase(it); }
		}	

		FUSE_INLINE bool exits(entity_id entity) {	
			return std::find(_data.begin(), _data.end(), entity) != _data.end();
		}

		FUSE_INLINE T& get(entity_id entity) {
			FUSE_ASSERT(this->exits(entity) && "entity out of range!");
			return (*std::find(_data.begin(), _data.end(), entity)).data;
		}

	private:		
		std::vector<component_entry<T>> _data;
	};
}