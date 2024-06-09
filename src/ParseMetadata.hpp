
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
  std::vector<std::string> m_errors;
  std::chrono::steady_clock::time_point m_start;

public:
  ParseMetadata();

  [[nodiscard]] std::chrono::seconds duration() const;

  [[nodiscard]] std::uint64_t lines() const;

  [[nodiscard]] const std::vector<std::string> &errors() const;

  void addLine();

  void addError(const std::string &);
  void addError(std::string &&);
};
