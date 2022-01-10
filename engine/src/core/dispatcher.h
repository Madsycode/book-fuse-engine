#pragma once
#include "event.h"

namespace fuse {
  struct dispatcher {
    // posts an event with the type T
    template <typename T, typename... Args>
    FUSE_INLINE void post(Args &&...args) {
      FUSE_STATIC_ASSERT(std::is_base_of<event_instance, T>::value);
      _events[get_typeid<T>()] = new T(std::forward<Args>(args)...);
    }

    // registers a callback for the event of type T
    template <typename T>
    FUSE_INLINE void add_callback(callback_function<T> &&clbk) {
      FUSE_STATIC_ASSERT(std::is_base_of<event_instance, T>::value);
      auto entry = std::make_shared<event_callback<T>>(clbk);
      _actions[get_typeid<T>()].push_back(std::move(entry));
    }

    // handles all event and keep memory clean
    FUSE_INLINE void dispatch() {
      if (!_events.size()) { return; }
      
      for (auto &[code, entry] : _events) {
        for (auto &callback : _actions[code]) {
          if (callback->execute(entry)) { break; }
        }
      }
      _events.clear();
    }

  private:
    std::unordered_map<type_id, event_instance *> _events;
    std::unordered_map<type_id, callback_list> _actions;
  };
}
