#pragma once

namespace fuse::math {
  template <typename T>
  struct vec2 {
    T x = 0, y = 0;

    FUSE_INLINE vec2(T x, T y) : x(x), y(y) {}
    FUSE_INLINE vec2(T v) : x(v), y(v) {}
    FUSE_INLINE vec2() = default;

    // operator (+)
    FUSE_INLINE vec2<T> operator+(const vec2<T> &v2) const {
      return vec2<T>(this->x + v2.x, this->y + v2.y);
    }
    // operator (+=)
    FUSE_INLINE vec2<T> &operator+=(const vec2<T> &v2) {
      this->x += v2.x;
      this->y += v2.y;
      return *this;
    }
    // operator (-)
    FUSE_INLINE vec2<T> operator-(const vec2<T> &v2) const {
      return vec2<T>(x - v2.x, y - v2.y);
    }
    // operator (-=)
    FUSE_INLINE vec2<T> &operator-=(const vec2<T> &v2) {
      this->x -= v2.x;
      this->y -= v2.y;
      return *this;
    }
    // operator (*)
    FUSE_INLINE vec2<T> operator*(T s) const {
      return vec2<T>(x * s, y * s);
    }
    // operator (*=)
    FUSE_INLINE vec2<T> &operator*=(T scalar) {
      this->x *= scalar;
      this->y *= scalar;
      return *this;
    }
  };
}

namespace fuse {
  using vec2u = math::vec2<uint32_t>;
  using vec2f = math::vec2<float>;
  using vec2i = math::vec2<int>;
}