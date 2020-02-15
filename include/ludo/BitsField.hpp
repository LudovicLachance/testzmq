#pragma once
#include <bits/locale_facets.h>
#include <type_traits>
using namespace std;

template <class bits>
constexpr bits operator~(const bits& f) {
  using Type = underlying_type_t<bits>;
  return bits{~static_cast<Type>(f)};
}
template <class bits>
constexpr bits operator|(const bits& lhs, const bits& rhs) {
  using Type = underlying_type_t<bits>;
  return bits{static_cast<Type>(lhs) | static_cast<Type>(rhs)};
}
template <class bits>
constexpr bits operator&(const bits& lhs, const bits& rhs) {
  using Type = underlying_type_t<bits>;
  return bits{static_cast<Type>(lhs) & static_cast<Type>(rhs)};
}
template <class bits>
constexpr bits operator^(const bits& lhs, const bits& rhs) {
  using Type = underlying_type_t<bits>;
  return bits{static_cast<Type>(lhs) ^ static_cast<Type>(rhs)};
}
template <class bits>
constexpr bits operator|=(const bits& lhs, const bits& rhs) {
  return lhs | rhs;
}
template <class bits>
constexpr bits operator&=(const bits& lhs, const bits& rhs) {
  return lhs & rhs;
}
template <class bits>
constexpr bits operator^=(const bits& lhs, const bits& rhs) {
  return lhs ^ rhs;
}

namespace ludo {

namespace BitsField {

template <class bits>
struct Runtime {
  typedef bits Type;

  template <class... Ts>
  static auto set(Ts... ts) {
    return (... | ts);
  }

  template <class... Ts>
  static auto same(Ts... ts) {
    return (... & ts);
  }

  template <class... Ts>
  static auto toggle(Ts... ts) {
    return (... ^ ts);
  }

  template <class T, class... Ts>
  static auto unset(T t, Ts... ts) {
    return (t &= ... &= ~ts);
  }

  template <class... Ts>
  static auto isset(Ts... ts) {
    return same(ts...) != Type::empty;
  }
};

template <class bits>
struct Compile {
  typedef bits Type;

  template <Type... Ts>
  static constexpr auto set() {
    return (... | Ts);
  }

  template <Type... Ts>
  static constexpr auto same() {
    return (... & Ts);
  }

  template <Type... Ts>
  static constexpr auto toggle() {
    return (... ^ Ts);
  }

  template <Type T, Type... Ts>
  static constexpr auto unset() {
    return (T &= ... &= ~Ts);
  }

  template <Type... Ts>
  static constexpr auto isset() {
    return same<Ts...>() != Type::empty;
  }
};

}  // namespace BitsField

}  // namespace ludo
