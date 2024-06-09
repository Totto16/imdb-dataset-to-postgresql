#
#pragma once

#include <functional>
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

} // namespace ParserHelper

class StaticParsers {
public:
  [[nodiscard]] static bool isNulledValue(const std::string &str);

  [[nodiscard]] static std::string asIs(const std::string &str);

  [[nodiscard]] static std::optional<std::string>
  asIsNullable(const std::string &str);

  [[nodiscard]] static bool booleanParser(const std::string &str);

  [[nodiscard]] static double doubleParser(const std::string &str);

  [[nodiscard]] static std::int64_t intParser(const std::string &str);

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

}; // namespace Parser
