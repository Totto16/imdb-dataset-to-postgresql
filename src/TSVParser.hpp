
#pragma once

#include <csv/parser.hpp>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <postgres/Postgres.h>
#include <string>

#include "ParseMetadata.hpp"
#include "ParserStructure.hpp"
#include "helper/expected.hpp"

using OmitHeadType = std::optional<bool>;

using ParseResult = helper::expected<ParseMetadata, std::string>;

using ParserMap = std::map<std::string, std::shared_ptr<Parseable>>;

class TSVParser {
public:
  TSVParser(std::filesystem::path file, std::string type, OmitHeadType hasHead,
            std::shared_ptr<Parseable> structure, off_t offset,
            std::size_t length);
  TSVParser(const TSVParser &) = default;
  ~TSVParser() = default;

  ParseResult parseData(postgres::Connection &connection, ParseOptions options);
  [[nodiscard]] static ParserMap getParserMap();

  [[nodiscard]] bool isHeader(const csv::record &record);

private:
  std::filesystem::path m_file;
  std::string m_type;
  OmitHeadType m_hasHead;
  std::shared_ptr<Parseable> m_structure;
  off_t m_offset;
  std::size_t m_length;
};

using MaybeParser = tl::expected<TSVParser, std::string>;

MaybeParser makeParser(std::filesystem::path file,
                       std::optional<std::string> optionalType,
                       OmitHeadType hasHead, off_t offset = 0,
                       std::size_t length = 0);
