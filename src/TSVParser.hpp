
#pragma once

#include <csv/parser.hpp>
#include <expected.hpp>
#include <map>
#include <memory>
#include <nan.h>
#include <string>
#include <vector>

#include "ParserStructure.hpp"
#include "types.hpp"

using namespace std;
using namespace v8;

// ts: export type OmitHeadType = "auto" | boolean
typedef enum { False = 0, True = 1, Auto = 2 } OmitHeadType;

typedef tl::expected<vector<int>, string> ParseResult;

typedef map<string, shared_ptr<ParserStructure>> ParserMap;

class TSVParser {
public:
  TSVParser(string path, string type, shared_ptr<ParserStructure> structure);
  TSVParser(const TSVParser &) = default;
  ~TSVParser() = default;

  ParseResult parseData();
  [[nodiscard]] static ParserMap getParserMap();

private:
  string m_path;
  string m_type;
  shared_ptr<ParserStructure> m_structure;
};

using MaybeParser = tl::expected<TSVParser, string>;

MaybeParser makeParser(string path, string type);
