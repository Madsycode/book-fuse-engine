#pragma once

#include "asset.h"

namespace fuse {
  struct audio_instance {
    Mix_Chunk* data = NULL;
    std::string filename;
  };

  struct audio_asset : asset {
    FUSE_INLINE audio_asset(const std::string& name, const audio_instance& audio) {
      this->audio = audio;
      this->name = name;
    }

    FUSE_INLINE audio_asset() = default;

    FUSE_INLINE ~audio_asset() {
      Mix_FreeChunk(audio.data);
    }

    audio_instance audio;
  };
}