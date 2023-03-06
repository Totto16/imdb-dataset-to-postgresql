

#pragma once

#include <memory>

#include "ParserStructure.hpp"

using namespace std;

class Model {
public:
  static shared_ptr<ParserStructure> mappedTitleAlternate();
  static shared_ptr<ParserStructure> mappedNameBasic();
  static shared_ptr<ParserStructure> mappedTitleBasic();
  static shared_ptr<ParserStructure> mappedTitleCrew();
  static shared_ptr<ParserStructure> mappedTitleEpisode();
  static shared_ptr<ParserStructure> mappedTitlePrincipal();
  static shared_ptr<ParserStructure> mappedTitleRating();

}; // namespace Model
