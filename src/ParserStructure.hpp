
#pragma once

#include <csv/parser.hpp>
#include <expected.hpp>
#include <functional>
#include <nan.h>
#include <string>
#include <vector>

#include "InternalParsers.hpp"
#include "constructable.hpp"



class ParserStructure {
public:
  ParserStructure(vector<string> order, vector<ParserFunction> declarations);
  ~ParserStructure() = default;

  ObjectValues parse(const csv::record &record);

  vector<string> m_order;

private:
  vector<ParserFunction> m_declarations;
};
