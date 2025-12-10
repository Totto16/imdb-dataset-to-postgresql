
#include "ParseMetadata.hpp"
#include <chrono>

ParseMetadata::ParseMetadata() : m_lines{}, m_errors{} {}

[[nodiscard]] std::uint64_t ParseMetadata::lines() const { return m_lines; }

[[nodiscard]] std::uint64_t ParseMetadata::errors() const { return m_errors; }

void ParseMetadata::addLine() { m_lines++; }

void ParseMetadata::addLines(std::uint64_t lines) { m_lines += lines; }

void ParseMetadata::addError() { m_errors++; }

void ParseMetadata::addErrors(std::uint64_t errors) { m_errors += errors; }

[[nodiscard]] std::chrono::milliseconds ParseResult::duration() const {
  const auto now = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start);
}

ParseResult::ParseResult()
    : m_result{std::unexpected<std::string>{"not initialized"}},
      m_start{std::chrono::steady_clock::now()} {}

[[nodiscard]] ParseMetadata ParseResult::value() const {
  return m_result.value();
}

[[nodiscard]] bool ParseResult::has_value() const {
  return m_result.has_value();
}

[[nodiscard]] const std::string &ParseResult::error() const {
  return m_result.error();
}

void ParseResult::set_value(const ParseMetadata &metadata) {
  this->m_result = metadata;
}

void ParseResult::set_error(const std::string &error) {
  this->m_result = std::unexpected<std::string>{error};
}
