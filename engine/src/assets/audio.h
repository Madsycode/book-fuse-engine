#pragma once

#include "asset_instance.h"

namespace fuse {
  struct audio_instance {
    Mix_Chunk *data = NULL;
    int volume = 0;
  };

  struct audio_asset : asset_instance {
    FUSE_INLINE audio_asset(const std::string& path, const std::string& name, const audio_instance& data) {
      this->id = generate_uuid();
      this->audio = data;
      this->name = name;
      this->path = path;
    }

    FUSE_INLINE audio_asset() = default;

    FUSE_INLINE ~audio_asset() {
      Mix_FreeChunk(audio.data);
    }

    audio_instance audio;
  };
}