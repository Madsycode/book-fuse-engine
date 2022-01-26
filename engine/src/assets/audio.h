#pragma once

#include "asset_instance.h"

namespace fuse {
  struct audio_asset : asset_instance {
    FUSE_INLINE audio_asset(const std::string& path, const std::string& name, Mix_Chunk* audio) {
      this->id = generate_uuid();
      this->audio = audio;
      this->name = name;
      this->path = path;
    }

    FUSE_INLINE audio_asset() = default;

    FUSE_INLINE ~audio_asset() {
      Mix_FreeChunk(audio);
    }

    Mix_Chunk* audio = NULL;
  };
}