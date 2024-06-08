
#pragma once



#include <functional>
#include <memory>
#include <string>


using ParserFunction = function<shared_ptr<Constructable>(const string &str)>;

class StaticParsers {
public:
  [[nodiscard]] static bool isNulledValue(const string &str);

  [[nodiscard]] static shared_ptr<Constructable> asIs(const string &str);

  [[nodiscard]] static shared_ptr<Constructable>
  imdbIdParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  alternativeTitleParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  titleTypeParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable> genreParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  nameIDParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  regionParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  languageParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  booleanParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable> floatParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable> intParser(const string &str);

  [[nodiscard]] static ParserFunction orNullParser(const ParserFunction &fn);
  [[nodiscard]] static ParserFunction arrayParser(const ParserFunction &fn);

}; // namespace Parser

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
