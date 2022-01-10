#pragma once 

namespace fuse {
    using type_id = unsigned int;	
    type_id get_unique_typeid();

	template<typename T>
	FUSE_INLINE type_id get_typeid() {
		static const type_id s_typeid = get_unique_typeid();
		return s_typeid;
	}
}