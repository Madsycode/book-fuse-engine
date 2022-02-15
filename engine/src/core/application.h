#pragma once

namespace fuse {
  struct app_config {
    int width = 0;
    int height = 0;       
    std::string title;
    std::string scene;
    std::string version;
  };

  // will be defined in game.exe
  app_config create_application();

  FUSE_API void run_application(const app_config&);  
}
