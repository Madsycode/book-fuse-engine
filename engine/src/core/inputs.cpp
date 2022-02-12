#include "pch.h"
#include "inputs.h"
#include "events/system.h"

namespace fuse::inputs {
  static const uint8_t* keyboard = NULL;
  static mouse_state mouse;
  static dispatcher disp;

  dispatcher* get_dispatcher() { return &disp; }
  bool is_key(int key) { return keyboard[key]; }

  const vec2f& mouse_wheel() { return mouse.wheel; }
  const vec2f& mouse_offset() { return mouse.offset; }
  bool is_button(int b) { return mouse.buttons.test(b); }

  void initialize(SDL_Window* window) { 
    keyboard = SDL_GetKeyboardState(NULL); 
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
      case SDL_MOUSEWHEEL: 
        mouse.wheel = vec2f(event.button.x, event.button.y);
        disp.post<mousewheel_event>(); break;
      case SDL_MOUSEMOTION: 
        mouse.offset = vec2f(event.button.x, event.button.y);
        disp.post<mousemotion_event>(); break;
      case SDL_KEYDOWN: 
        keyboard = SDL_GetKeyboardState(NULL);
        disp.post<keydown_event>(event.key.keysym.scancode);
      case SDL_KEYUP: 
        disp.post<keyup_event>(event.key.keysym.scancode);
        keyboard = SDL_GetKeyboardState(NULL); break; 
      }
    }
    disp.dispatch();
  }
}
