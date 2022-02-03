#pragma once
#include "core/typeid.h"

namespace fuse::ecs {
	using entityid = size_t; 
  using componentid = uint32_t;
	using signature = std::set<uint32_t>; 
	using entitylist = std::set<entityid>; 

  // component array interface
  struct array_instance {
		FUSE_INLINE virtual ~array_instance() = default;
		virtual void erase(entityid) = 0;
	};
  // component array's data entry
	template<typename T> struct array_entry {
		FUSE_INLINE array_entry(entityid e, const T& d): entity(e), data(d) {}
		FUSE_INLINE operator entityid() const { return entity; }
		entityid entity = INVALID_ID;
		T data;
	};

  // component array
	template<typename T>
	struct component_array : array_instance {				
		FUSE_INLINE T& push(entityid e, const T& data) {
			if (exits(e)) { return get(e); }
			_data.push_back(array_entry(e, data));
			return _data.back().data;			
		}	

		FUSE_INLINE void erase(entityid e) {	
			auto it = std::find(_data.begin(), _data.end(), e);
			if (it != _data.end()) { _data.erase(it); }
		}	

		FUSE_INLINE bool exits(entityid e) {	
			return std::find(_data.begin(), _data.end(), e) != _data.end();
		}

		FUSE_INLINE T& get(entityid e) {
			FUSE_ASSERT(exits(e) && "entity out of range!");
			return (*std::find(_data.begin(), _data.end(), e)).data;
		}

	private:		
		std::vector<array_entry<T>> _data;
	};
}