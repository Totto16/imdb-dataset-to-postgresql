

#pragma once

#include <memory>
#include <type_traits>

#include "ParserStructure.hpp"

#define MAKE_PARSER(T, name, parser)                                           \
  AdvancedParser<T> {                                                          \
    #name, [](T &value, const std::string &input) {                            \
      using ReturnType =                                                       \
          std::invoke_result<decltype(parser), const std::string &>::type;     \
      static_assert(std::is_same_v<decltype(T::name), ReturnType>);            \
      value.name = parser(input);                                              \
    }                                                                          \
  }

class Model {
public:
  static std::shared_ptr<Parseable> mappedTitleAlternate();
  static std::shared_ptr<Parseable> mappedNameBasic();
  static std::shared_ptr<Parseable> mappedTitleBasic();
  static std::shared_ptr<Parseable> mappedTitleCrew();
  static std::shared_ptr<Parseable> mappedTitleEpisode();
  static std::shared_ptr<Parseable> mappedTitlePrincipal();
  static std::shared_ptr<Parseable> mappedTitleRating();

}; // namespace Model
