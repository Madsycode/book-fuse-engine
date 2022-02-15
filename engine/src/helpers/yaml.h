#pragma once
#include "math/vec2.h"
#include "yaml-cpp/yaml.h"

namespace YAML {
    template <>
    struct convert<fuse::vec2f> {
        FUSE_INLINE static Node encode(const fuse::vec2f &rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        FUSE_INLINE static bool decode(const Node &node, fuse::vec2f &rhs) {
            if (!node.IsSequence() || node.size() != 2) {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    // stream operator
    FUSE_INLINE Emitter &operator<<(Emitter &emitter, const fuse::vec2f &v) {
        emitter << Flow;
        emitter << BeginSeq << v.x << v.y << EndSeq;
        return emitter;
    }
}