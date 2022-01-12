#pragma once

namespace fuse::ecs {    
    struct tag_component {
        FUSE_INLINE tag_component(const std::string& tag): tag(tag) {}
        FUSE_INLINE tag_component(const tag_component&) = default;
        FUSE_INLINE tag_component() = default;
        std::string tag;
    };
}