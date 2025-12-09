
#pragma once

#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <string>

template <typename T>
using ParserFunction = std::function<T(const std::string &str)>;

namespace ParserHelper {

// from:
// https://stackoverflow.com/questions/10058606/splitting-a-string-by-a-character
template <template <typename, typename> class Container, typename Separator>
Container<std::string, std::allocator<std::string>>
split(const std::string &line, Separator sep) {
  std::size_t pos = 0;
  std::size_t next = 0;
  Container<std::string, std::allocator<std::string>> fields;
  while (next != std::string::npos) {
    next = line.find_first_of(sep, pos);
    std::string field = next == std::string::npos
                            ? line.substr(pos)
                            : line.substr(pos, next - pos);
    fields.push_back(field);
    pos = next + 1;
  }
  return fields;
}

// from
// https://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c
enum class STR2INT_ERROR { SUCCESS, OVERFLOW, UNDERFLOW, INCONVERTIBLE };

using ParseIntType = std::int64_t;

inline STR2INT_ERROR str2int(ParseIntType &l, char const *s, int base = 10) {
  char *end;
  errno = 0;
  // not using std::strtol, since that throws exceptions!
  l = strtol(s, &end, base);
  if ((errno == ERANGE && l == LONG_MAX)) {
    return STR2INT_ERROR::OVERFLOW;
  }
  if ((errno == ERANGE && l == LONG_MIN)) {
    return STR2INT_ERROR::UNDERFLOW;
  }
  if (*s == '\0' || *end != '\0') {
    return STR2INT_ERROR::INCONVERTIBLE;
  }
  return STR2INT_ERROR::SUCCESS;
}

} // namespace ParserHelper

class StaticParsers {
public:
  [[nodiscard]] static bool isNulledValue(const std::string &str);

  [[nodiscard]] static std::string asIs(const std::string &str);

  [[nodiscard]] static std::optional<std::string>
  asIsNullable(const std::string &str);

  [[nodiscard]] static bool booleanParser(const std::string &str);

  [[nodiscard]] static double doubleParser(const std::string &str);

  [[nodiscard]] static float floatParser(const std::string &str);

  template <std::integral T>
  [[nodiscard]] static T intParser(const std::string &str) {
    ParserHelper::ParseIntType i;
    const auto result = ParserHelper::str2int(i, str.c_str());
    if (result != ParserHelper::STR2INT_ERROR::SUCCESS) {
      throw std::runtime_error("Couldn't parse int of '" + str + "'");
    }

    static_assert(std::numeric_limits<T>::is_signed ==
                  std::numeric_limits<ParserHelper::ParseIntType>::is_signed);

    static_assert(std::numeric_limits<T>::max() <=
                  std::numeric_limits<ParserHelper::ParseIntType>::max());

    if (i > std::numeric_limits<T>::max()) {
      throw std::runtime_error("integer out of bounds, to big");
    }

    if (i < std::numeric_limits<T>::min()) {
      throw std::runtime_error("integer out of bounds, to small");
    }

    return static_cast<T>(i);
  }

  template <typename T>
  [[nodiscard]] static ParserFunction<std::optional<T>>
  orNullParser(const ParserFunction<T> &fn) {
    return [fn](const std::string &str) -> std::optional<T> {
      if (StaticParsers::isNulledValue(str)) {
        return std::nullopt;
      }

      return fn(str);
    };
  }

  template <typename T>
  [[nodiscard]] static ParserFunction<std::vector<T>>
  arrayParser(const ParserFunction<T> &fn) {

    return [fn](const std::string &str) -> std::vector<T> {
      std::vector<T> vec = {};

      if (StaticParsers::isNulledValue(str)) {
        return vec;
      }

      auto parts = ParserHelper::split<std::vector>(str, ",");
      for (const auto &value : parts) {
        vec.push_back(fn(value));
      }

      return vec;
    };
  }

  template <typename T>
  [[nodiscard]] static T enumParser(const std::string &str) {
    auto result = asIs(str);
    return T{{std::move(result)}};
  }

}; // namespace Parser
