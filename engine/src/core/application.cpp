#include "pch.h"
#include "inputs.h"
#include "ecs/scene.h"
#include "application.h"
#include "events/system.h"

namespace fuse {
  static float deltatime, last_tick; 
  static ecs::scene* scene = NULL;
  static bool is_running = true;
  static app_config config;

  FUSE_INLINE void compute_deltatime() {
    deltatime = get_ticks() - last_tick;
    if(deltatime > MAX_DELTATIME) {
      deltatime = MAX_DELTATIME;
    }
    last_tick = get_ticks();    
  }

  FUSE_INLINE bool on_quit(const quit_event& e) { 
    return is_running = false; 
  }

  void run_application(const app_config& cfg) {
    // set config
    config = cfg;

    // init SDL's systems
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
    if (!window) {
      FUSE_ERROR("%s", SDL_GetError());
      exit(EXIT_FAILURE);
    }

    // create renderer 
    auto r_flags = (SDL_RendererFlags)(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, r_flags);
    if (!renderer) {
      FUSE_ERROR("%s", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    
    // initialize inputs
    inputs::initialize(window);

    // register event callbacks
    auto disp = inputs::get_disp();
    disp->add_callback<quit_event>(on_quit);

    // create & load scene
    scene = new ecs::scene(renderer);
    scene->deserialize(config.scenepath);

    // start time 
    last_tick = (float)get_ticks();

    // game loop
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
    Mix_Quit();
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
  }
}