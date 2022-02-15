#include <core/entry.h>

fuse::app_config fuse::create_application() {
  fuse::app_config config;
  config.scene = "assets/scene.yaml";
  config.title = "Flappy Plane";
  config.version = "0.0.1";
  config.width = 380;
  config.height = 720;
  return config;
}