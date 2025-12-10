
#pragma once

#include <chrono>
#include <cstdint>

#include "./helper/cli_arguments.hpp"

struct ParseOptions {
  IgnoreErrors ignoreErrors;
  bool verbose;
};

struct ParseMetadata {
private:
  std::uint64_t m_lines;
  std::uint64_t m_errors;

public:
  ParseMetadata();
  [[nodiscard]] std::uint64_t lines() const;

  [[nodiscard]] std::uint64_t errors() const;

  void addLine();
  void addLines(std::uint64_t lines);

  void addError();
  void addErrors(std::uint64_t errors);
};

struct ParseResult {
private:
  std::expected<ParseMetadata, std::string> m_result;
  std::chrono::steady_clock::time_point m_start;

public:
  explicit ParseResult();

  [[nodiscard]] std::chrono::milliseconds duration() const;

  [[nodiscard]] ParseMetadata value() const;

  [[nodiscard]] bool has_value() const;

  [[nodiscard]] const std::string &error() const;

  void set_value(const ParseMetadata &metadata);

  void set_error(const std::string &error);
};
