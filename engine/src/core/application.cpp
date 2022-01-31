#include "pch.h"
#include "inputs.h"
#include "application.h"
#include "events/system.h"

#include "math/vec2.h"

namespace fuse {
  static bool is_running = true;

  FUSE_INLINE bool on_quit(const quit_event&) { 
    return is_running = false; 
  }

  FUSE_API void run_application(const app_config& config) {
    // init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      FUSE_ERROR("%s", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    // init SDL_image
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0) {
      FUSE_ERROR("%s", IMG_GetError());
    }
    // init SDL_ttf
    if(TTF_Init() < 0) { 
      FUSE_ERROR("%s", TTF_GetError());         
    }
    // init SDL_mixer
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) < 0 || 
      Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512)) {
      FUSE_ERROR("%s", Mix_GetError());
    }

    // create window
    auto w_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(config.title.c_str(), SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, config.width, config.height, w_flags);

    // create renderer 
    auto r_flags = (SDL_RendererFlags)(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    auto renderer = SDL_CreateRenderer(window, -1, r_flags);
    if (!window || !renderer) {
      FUSE_ERROR("%s", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    
    vec2f a(1.0f);
    vec2i b(2,4);
    
    inputs::initialize(window);
    inputs::get_dispatcher()->add_callback<quit_event>(on_quit);

    while (is_running) { 
      if(inputs::is_button(SDL_BUTTON_LEFT)) {
        auto mouse = inputs::mouse_offset();
        FUSE_INFO("(%f, %f)", mouse.x, mouse.y);
      }
      inputs::dispatch_events();
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
}

    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
  }
}