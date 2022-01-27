#pragma once

namespace fuse::application {
    struct app_configs {
        int width = SCREEN_WIDTH;
        int height = SCREEN_HEIGHT;       
        std::string title = "Fuse2D";
        std::string version = "0.0.1";
        std::string scenepath = "scene.yaml";
    };

    FUSE_API void run();  
}