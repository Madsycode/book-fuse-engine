#include "pch.h"
#include "inputs.h"
#include "application.h"
#include "events/system.h"

namespace fuse {
  static bool is_running = true;

  bool on_quit(const quit_event&) { 
    is_running = false; 
    return true;
  }

  bool on_resized(const resized_event& e) { 
    FUSE_INFO("window resized %d %d", e.width, e.height)
    return true;
  }

  void start_application() {
    app_configuration config;

    // init sdl systems
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      FUSE_ERROR("SDL_Init failed: %s", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    // create window
    auto w_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow(config.name, config.start_x, config.start_y, config.width, config.height, w_flags);
    if (!window) {
      FUSE_ERROR("SDL_CreateWindow failed: %s", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    // create renderer context
    auto r_flags = (SDL_RendererFlags)(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, r_flags);
    if (!renderer) {
      FUSE_ERROR("SDL_CreateRenderer failed: %s", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    // register callbacks
    auto disp = inputs::get_dispatcher();
    disp->add_callback<resized_event>(on_resized);
    disp->add_callback<quit_event>(on_quit);

    while (is_running) { 
      inputs::process_sdl_inputs(); 
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
}