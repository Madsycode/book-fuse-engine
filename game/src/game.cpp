#include "core/entry.h"

fuse::app_config fuse::create_application() {
  fuse::app_config config;
  config.scenepath = "assets/scene.yaml";
  config.version = "0.0.1";
  config.title = "Fuse2D";
  config.height = 720;
  config.width = 380;
  return config;
}