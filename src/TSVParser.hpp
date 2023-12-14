
#pragma once

#include <csv/parser.hpp>
#include <expected.hpp>
#include <map>
#include <memory>
#include <nan.h>
#include <string>
#include <vector>

#include "ParserStructure.hpp"
#include "helper.hpp"

using namespace std;
using namespace v8;

// ts: export type OmitHeadType = "auto" | boolean
enum class OmitHeadType { False = 0, True = 1, Auto = 2 };

using ParseResult = tl::expected<int32_t, string>;

using ParserMap = map<string, shared_ptr<ParserStructure>>;

class TSVParser {
public:
  TSVParser(string path, string type, OmitHeadType hasHead,
            shared_ptr<ParserStructure> structure);
  TSVParser(const TSVParser &) = default;
  ~TSVParser() = default;

  ParseResult parseData(const ExecutionProgressSender *sender,
                        EventEmitterFunctionReentrant emitter);
  [[nodiscard]] static ParserMap getParserMap();

  [[nodiscard]] bool isHeader(const csv::record &record);

private:
  string m_path;
  string m_type;
  OmitHeadType m_hasHead;
  shared_ptr<ParserStructure> m_structure;
};

using MaybeParser = tl::expected<TSVParser, string>;

MaybeParser makeParser(string path, string type, OmitHeadType hasHead);
