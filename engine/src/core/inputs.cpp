#include "pch.h"
#include "inputs.h"
#include "events/system.h"

namespace fuse::inputs {
    static const uint8_t *keyboard = SDL_GetKeyboardState(NULL);
    static dispatcher dispatchr;

    static struct mouse_state {
        std::bitset<7> buttons = {0};
        vec2i position;
        vec2i wheel;
    } mouse;

    vec2i mouse_position() { return mouse.position; }
    vec2i mouse_wheel() { return mouse.wheel; }

    bool is_pressed(int key) { return keyboard[key]; }
    dispatcher* get_dispatcher() { return &dispatchr; }
    bool is_down(int button) { return mouse.buttons.test(button); }

    void process_sdl_inputs() {
      static SDL_Event event;

      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: dispatchr.post<quit_event>(); break;

        case SDL_MOUSEBUTTONDOWN: 
          mouse.buttons.set(event.button.button);
          dispatchr.post<mousedown_event>(event.button.button); break;

        case SDL_MOUSEBUTTONUP: 
          mouse.buttons.reset(event.button.button);
          dispatchr.post<mouseup_event>(event.button.button); break;

        case SDL_MOUSEMOTION:           
          mouse.position = vec2i(event.button.x, event.button.y);
          dispatchr.post<mousemotion_event>(); break;

        case SDL_MOUSEWHEEL:           
          mouse.wheel = vec2i(event.button.x, event.button.y);
          dispatchr.post<mousewheel_event>(); break;

        case SDL_KEYDOWN: 
          keyboard = SDL_GetKeyboardState(NULL);
          if (event.key.repeat == 0) {
            dispatchr.post<keydown_event>(event.key.keysym.scancode);
          }
          else {
            dispatchr.post<keyrepeat_event>(event.key.keysym.scancode);                        
          } break;

        case SDL_KEYUP: 
          dispatchr.post<keyup_event>(event.key.keysym.scancode);
          keyboard = SDL_GetKeyboardState(NULL); break;

        case SDL_WINDOWEVENT: 
          if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
            int width = event.window.data1;
            int height = event.window.data2;
            dispatchr.post<resized_event>(width, height);
          } break; 
        }
      }
      dispatchr.dispatch();
  }
}