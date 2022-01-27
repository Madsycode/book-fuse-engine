#include "pch.h"
#include "inputs.h"
#include "events/system.h"

namespace fuse::inputs {
    static const uint8_t* keyboard = NULL;
    static mouse_state mouse;
    static dispatcher disp;

    static int display_w = 0;
    static int display_h = 0;

    // ++

    dispatcher* get_disp() { return &disp; }
    const vec2f& mouse_wheel() { return mouse.wheel; }
    const vec2f& mouse_offset() { return mouse.offset; }
    const vec2f& display_size() { return vec2f(display_w, display_h); }

    bool is_key(int key) { return keyboard[key]; }
    bool is_button(int b) { return mouse.buttons.test(b); }

    void initialize(SDL_Window* window) {
      keyboard = SDL_GetKeyboardState(NULL);
      SDL_GetWindowSize(window, &display_w, &display_h);
    }

    void dispatch_events() {
      static SDL_Event event;
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: disp.post<quit_event>(); break;

        case SDL_MOUSEBUTTONDOWN: 
          mouse.buttons.set(event.button.button);
          disp.post<mousedown_event>(event.button.button); break;

        case SDL_MOUSEBUTTONUP: 
          mouse.buttons.reset(event.button.button);
          disp.post<mouseup_event>(event.button.button); break;

        case SDL_MOUSEMOTION:           
          mouse.offset = vec2f(event.button.x, event.button.y);
          disp.post<mousemotion_event>(); break;

        case SDL_MOUSEWHEEL:           
          mouse.wheel = vec2f(event.button.x, event.button.y);
          disp.post<mousewheel_event>(); break;

        case SDL_KEYDOWN: 
          keyboard = SDL_GetKeyboardState(NULL);
          if (event.key.repeat == 0) {
            disp.post<keydown_event>(event.key.keysym.scancode);
          }
          else {
            disp.post<keyrepeat_event>(event.key.keysym.scancode);                        
          } break;

        case SDL_KEYUP: 
          disp.post<keyup_event>(event.key.keysym.scancode);
          keyboard = SDL_GetKeyboardState(NULL); break;

        case SDL_WINDOWEVENT: 
          if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            if(event.window.data1 && event.window.data2) {
              display_w = event.window.data1;
              display_h = event.window.data2;
              disp.post<resized_event>(display_w, display_h);
            }
          } break; 
        }
      }
      disp.dispatch();
  }
}