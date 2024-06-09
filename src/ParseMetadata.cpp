
#include "ParseMetadata.hpp"

ParseMetadata::ParseMetadata()
    : m_lines{}, m_errors{}, m_start{std::chrono::steady_clock::now()} {}

[[nodiscard]] std::chrono::seconds ParseMetadata::duration() const {
  const auto now = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::seconds>(now - m_start);
}

[[nodiscard]] std::uint64_t ParseMetadata::lines() const { return m_lines; }

[[nodiscard]] std::uint64_t ParseMetadata::errors() const { return m_errors; }

void ParseMetadata::addLine() { m_lines++; }

void ParseMetadata::addError() { m_errors++; }
