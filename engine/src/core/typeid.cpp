#include "pch.h"
#include "typeid.h"

namespace fuse {
    static type_id s_typeid = 1u;
    
    type_id get_unique_typeid() { 
        return s_typeid++; 
    }
}