#pragma once
#include <cstring>

namespace ludo {
template <class bits>
struct BitsConverter {
  union convert_u_t {
    char toChar[8];
    bits toCode;
  };

  static std::string toString(bits code) {
    convert_u_t converter;
    converter.toCode = code;
    return std::string{converter.toChar, sizeof(bits)};
  }

  static bits fromString(std::string str) {
    convert_u_t converter;
    std::strncpy(converter.toChar, str.data(), sizeof(bits));
    return converter.toCode;
  }
};
}  // namespace ludo