#include "pch.h"
#include "inputs.h"
#include "application.h"
#include "events/system.h"
#include "scene/scene_instance.h"

namespace fuse {
  static bool is_running = true;
  static app_configuration config;
  static float deltatime, last_tick; 

  void compute_deltatime() {
    float current_tick = (float)get_ticks_sec();
    deltatime = (current_tick - last_tick);
    last_tick = current_tick;    
  }

  bool on_quit(const quit_event&) { 
    return is_running = false; 
  }

  void start_application() {
    // init sdl systems
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      FUSE_ERROR("SDL_Init failed: %s", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    // create window
    auto w_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(config.name, config.start_x, config.start_y, config.width, config.height, w_flags);
    if (!window) {
      FUSE_ERROR("SDL_CreateWindow failed: %s", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    // create renderer context
    auto r_flags = (SDL_RendererFlags)(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, r_flags);
    if (!renderer) {
      FUSE_ERROR("SDL_CreateRenderer failed: %s", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    // register callbacks
    auto disp = inputs::get_dispatcher();
    disp->add_callback<quit_event>(on_quit);

    // create scene
    auto scene = new scene_instance(renderer, disp);
    scene->start();

    last_tick = (float)get_ticks_sec();

    while (is_running) { 
      compute_deltatime();
      inputs::process_events();  
      SDL_RenderClear(renderer);     
      scene->update(deltatime);  
      SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
}