
#pragma once

#include <csv/parser.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "ParserStructure.hpp"
#include "helper/expected.hpp"
#include "postgres.hpp"

using OmitHeadType = std::optional<bool>;

using ParseResult = helper::expected<std::int32_t, std::string>;

using ParserMap = std::map<std::string, std::shared_ptr<Parseable>>;

class TSVParser {
public:
  TSVParser(std::string path, std::string type, OmitHeadType hasHead,
            std::shared_ptr<Parseable> structure);
  TSVParser(const TSVParser &) = default;
  ~TSVParser() = default;

  ParseResult parseData(postgres::Connection &connection);
  [[nodiscard]] static ParserMap getParserMap();

  [[nodiscard]] bool isHeader(const csv::record &record);

private:
  std::string m_path;
  std::string m_type;
  OmitHeadType m_hasHead;
  std::shared_ptr<Parseable> m_structure;
};

using MaybeParser = tl::expected<TSVParser, std::string>;

MaybeParser makeParser(std::string path, std::string type,
                       OmitHeadType hasHead);
