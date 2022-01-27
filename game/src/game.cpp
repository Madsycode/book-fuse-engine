#include <fuse2d.h>

namespace game {
  struct game_configs {
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;       
    std::string title = "Fuse2D";
    std::string version = "v0.0.1";
    std::string scenepath = "scene/scene.yaml";
  };
}