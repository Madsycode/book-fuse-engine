#pragma once

namespace fuse {
    struct app_configuration {
        const char* name = "Fuse2D";
        const char* version = "0.0.1";
        int width = SCREEN_WIDTH;
        int height = SCREEN_HEIGHT;
        int start_x = SDL_WINDOWPOS_CENTERED;
        int start_y = SDL_WINDOWPOS_CENTERED;        
    };

    FUSE_API void start_application();  
}