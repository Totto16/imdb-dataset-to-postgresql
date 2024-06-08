

#pragma once

#include <memory>

#include "ParserStructure.hpp"

class Model {
public:
  static std::shared_ptr<ParserStructure> mappedTitleAlternate();
  static std::shared_ptr<ParserStructure> mappedNameBasic();
  static std::shared_ptr<ParserStructure> mappedTitleBasic();
  static std::shared_ptr<ParserStructure> mappedTitleCrew();
  static std::shared_ptr<ParserStructure> mappedTitleEpisode();
  static std::shared_ptr<ParserStructure> mappedTitlePrincipal();
  static std::shared_ptr<ParserStructure> mappedTitleRating();

}; // namespace Model
