
#pragma once

#include <chrono>
#include <cstdint>

struct ParseOptions {
  bool ignoreErrors;
  bool verbose;
};

struct ParseMetadata {
private:
  std::uint64_t m_lines;
  std::uint64_t m_errors;
  std::chrono::steady_clock::time_point m_start;

public:
  ParseMetadata();

  [[nodiscard]] std::chrono::seconds duration() const;

  [[nodiscard]] std::uint64_t lines() const;

  [[nodiscard]] std::uint64_t errors() const;

  void addLine();
  void addLines(std::uint64_t lines);

  void addError();
  void addErrors(std::uint64_t errors);
};
