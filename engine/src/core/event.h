#pragma once
#include "typeid.h"

namespace fuse {
  // base structure for all event types
  struct event_instance {
    FUSE_INLINE virtual ~event_instance() = default;
  };

  // cast an event pointer to a specific type
  template <typename T>
  FUSE_INLINE static const T &cast_event(event_instance *e) {
    FUSE_STATIC_ASSERT(std::is_base_of<event_instance, T>::value);
    return static_cast<const T &>(*e);
  }

  // event callback
  template <typename T>
  using callback_function = std::function<bool(const T &)>;

  struct callback_instance {
    FUSE_INLINE virtual ~callback_instance() = default;
    virtual bool execute(event_instance *) = 0;
  };

  template <typename T>
  struct event_callback : callback_instance {
    FUSE_INLINE event_callback(callback_function<T> &clbk) : callback(clbk) {}
    FUSE_INLINE bool execute(event_instance *e) {
      return callback(cast_event<T>(e));
    }
    callback_function<T> callback;
  };

  using callback_list = std::vector<std::shared_ptr<callback_instance>>;
}