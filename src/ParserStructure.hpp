
#pragma once

#include <csv/parser.hpp>
#include <expected.hpp>
#include <functional>
#include <nan.h>
#include <string>
#include <vector>

#include "InternalParsers.hpp"
#include "constructable.hpp"
#include "types.hpp"

using namespace std;
using namespace v8;

class ParserStructure {
public:
  ParserStructure(vector<string> order, vector<ParserFunction> declarations);
  ~ParserStructure() = default;

  ObjectValues parse(const csv::record &record);

private:
  vector<string> m_order;
  vector<ParserFunction> m_declarations;
};