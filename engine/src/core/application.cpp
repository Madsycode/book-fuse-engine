#include "pch.h"
#include "inputs.h"
#include "application.h"
#include "events/system.h"
#include "scene/scene_serializer.h"

namespace fuse::application {
  static scene_instance* scene = NULL;
  static float deltatime, last_tick; 
  static bool is_running = true;
  static app_configs cfg;

  void compute_deltatime() {
    float current_tick = (float)get_ticks_sec();
    deltatime = (current_tick - last_tick);
    last_tick = current_tick;    
  }

  bool on_quit(const quit_event&) { 
    return is_running = false; 
  }

  bool on_keydown(const keydown_event& e) { 
    if(inputs::is_pressed(SDL_SCANCODE_LCTRL)) {
      // serialize
      if(e.key == SDL_SCANCODE_S) {
        scene_serializer(scene).serialize("scene.yaml");
      }

      // deserialize
      if(e.key == SDL_SCANCODE_O) {
        scene_serializer(scene).deserialize("scene.yaml");
      }
    }
    return false;
  }

  void run() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      FUSE_ERROR("SDL_Init failed: %s", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    // create window
    auto w_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(cfg.title.c_str(), SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, cfg.width, cfg.height, w_flags);
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
    disp->add_callback<keydown_event>(on_keydown);

    // create scene
    scene = new scene_instance(renderer, disp);
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