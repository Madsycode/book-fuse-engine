#include "pch.h"
#include "application.h"

namespace fuse {
    static bool is_running = true;

    void start_application() {
        app_configuration config;           

        // init sdl systems     
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0) { 
            FUSE_ERROR("failed to initialize sdl's systems: %s", SDL_GetError()); 
            exit(EXIT_FAILURE);
        }        

        // create window
        auto w_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        SDL_Window* window = SDL_CreateWindow(config.name, config.start_x, config.start_y, config.width, config.height, w_flags);
        if(!window) {
            FUSE_ERROR("failed to create window: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        // create renderer context
        auto r_flags = (SDL_RendererFlags)(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, r_flags);
        if(!renderer) {
            FUSE_ERROR("failed to create window: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        while(is_running) {}
        
        SDL_Quit();
    }
}