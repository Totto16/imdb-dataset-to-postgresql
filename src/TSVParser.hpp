
#pragma once

#include <csv/parser.hpp>
#include <expected.hpp>
#include <nan.h>
#include <string>
#include <vector>

#include "types.hpp"

using namespace std;
using namespace v8;

typedef tl::expected<Local<Value>, string> ParseResult;

template <typename T> class TSVParser {
public:
  TSVParser(string path, string type);
  ~TSVParser() = default;
  ParseResult parseData();

private:
  string m_path;
  string m_type;
};

template <typename T> using MaybeParser = tl::expected<TSVParser<T> *, string>;