#include "pch.h"
#include "inputs.h"
#include "ecs/scene.h"
#include "application.h"
#include "events/system.h"

namespace fuse {
  static app_config config;
  static bool is_running = true;
  static ecs::scene* scene = NULL;
  static float deltatime, last_tick;

  FUSE_INLINE bool on_quit(const quit_event&) { 
    return is_running = false; 
  }

  FUSE_INLINE bool on_key(const keydown_event& e) { 
    if(e.key == SDL_SCANCODE_R) {
      scene->deserialize(config.scene);
    }
    return false;
  }

  FUSE_INLINE void compute_deltatime() { 
    deltatime = get_ticks() - last_tick;
    if(deltatime > MAX_DELTATIME) {
      deltatime = MAX_DELTATIME;
    }
    last_tick = get_ticks();
  }

  FUSE_API void run_application(const app_config& cfg) {
    // set config
    config = cfg;

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
    
    inputs::initialize(window);
    auto disp = inputs::get_dispatcher();
    disp->add_callback<quit_event>(on_quit);
    disp->add_callback<keydown_event>(on_key);

    // create & start scene
    scene = new ecs::scene(renderer);
    scene->deserialize(config.scene);

    last_tick = get_ticks();

    while (is_running) { 
      compute_deltatime();
      inputs::dispatch_events();
      SDL_RenderClear(renderer);
      scene->update(deltatime);
      SDL_RenderPresent(renderer);
    }

    // free memory
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    FUSE_DELETE(scene);
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
  }
}