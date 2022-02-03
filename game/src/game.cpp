#include <core/entry.h>

fuse::app_config fuse::create_application() {
  fuse::app_config config;
  config.version = "0.0.1";
  config.title = "Fuse2D";
  config.width = 1080;
  config.height = 720;
  return config;
}