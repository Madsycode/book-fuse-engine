#pragma once

#include "asset.h"

namespace fuse {
  struct audio {
    Mix_Chunk* data = NULL;
    std::string filename;
  };

  struct audio_asset : asset {
    FUSE_INLINE audio_asset(const std::string& name, const audio& data) {
      this->instance = data;
      this->name = name;
    }

    FUSE_INLINE audio_asset() = default;

    FUSE_INLINE ~audio_asset() {
      Mix_FreeChunk(instance.data);
    }

    audio instance;
  };
}