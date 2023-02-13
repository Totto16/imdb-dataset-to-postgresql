
#pragma once

#include <expected.hpp>
#include <functional>
#include <nan.h>
#include <string>
#include <vector>

#include "types.hpp"

using namespace std;
using namespace v8;

typedef function<Local<Value>(const string &str)> ParseFunction;

class ParserStructure {
public:
  ParserStructure(vector<string> order, vector<ParseFunction> declarations);
  ~ParserStructure() = default;

private:
  vector<string> m_order;
  vector<function<Local<Value>(const string &str)>> m_declarations;
};