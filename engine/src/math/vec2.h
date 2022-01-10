#pragma once

namespace fuse {
  namespace math {
    template <typename T>
    struct vec2 {
      T x = 0;
      T y = 0;

      vec2(T x, T y) : x(x), y(y) {}
      vec2(T v) : x(v), y(v) {}
      vec2() = default;

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

      // operator (==)
      FUSE_INLINE bool operator==(const vec2<T> &v2) const {
        return (this->x == v2.x && this->y == v2.y);
      }

      // operator (!=)
      FUSE_INLINE bool operator!=(const vec2<T> &v2) const {
        return (this->x != v2.x || this->y != v2.y);
      }

      // stream operator
      FUSE_INLINE friend std::ostream &operator<<(std::ostream &os, const vec2<T> &v) {
        return os << "(" << v.x << "," << v.y << ")";
      }
    };
  }

  // define vector types
  using vec2i = math::vec2<int>;
  using vec2f = math::vec2<float>;
  using vec2u = math::vec2<unsigned int>;
}