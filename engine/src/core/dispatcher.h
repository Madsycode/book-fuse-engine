#pragma once
#include "event.h"

namespace fuse {
  struct dispatcher {
    // posts an event with the type T
    template <typename T, typename... Args>
    FUSE_INLINE void post(Args&&...args) {
      FUSE_STATIC_ASSERT(std::is_base_of<event_instance, T>::value);
      _events[type_id<T>()] = new T(std::forward<Args>(args)...);
    }

    // registers a callback for the event of type T
    template <typename T>
    FUSE_INLINE void add_callback(callback_function<T>&& clbk) {
      FUSE_STATIC_ASSERT(std::is_base_of<event_instance, T>::value);
      auto entry = std::make_shared<event_callback<T>>(clbk);
      _callbacks[type_id<T>()].push_back(entry);
    }

    // handles all event and keep memory clean
    FUSE_INLINE void dispatch() {
      if (!_events.size()) { return; }      
      for (auto &[code, entry] : _events) {
        for (auto& cbk : _callbacks[code]) {
          if (cbk->run(entry)) { break; }
        }
      }
      _events.clear();
    }

  private:
    std::unordered_map<event_id, event_instance*> _events;
    std::unordered_map<event_id, callback_list> _callbacks;
  };
}
