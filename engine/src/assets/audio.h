#pragma once
#include "asset.h"

namespace fuse {
  struct audio_clip {
    Mix_Chunk* data = NULL;
    std::string filename;
  };

  struct audio_asset : asset {
    FUSE_INLINE audio_asset() = default;

    FUSE_INLINE ~audio_asset() {
      Mix_FreeChunk(instance.data);
    }

    audio_clip instance;
  };
}