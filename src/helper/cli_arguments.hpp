#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <vector>

enum class LogLevel : std::uint8_t {
  Error = 0,
  Warning,
  Info,
  Verbose,
};

enum class TableMode : std::uint8_t {
  DoNohing = 0,
  Check,
  CheckAndCreate,
};

struct IgnoreErrors {
private:
  std::expected<std::optional<std::string>, bool> m_underlying;

public:
  explicit IgnoreErrors() : m_underlying{std::unexpected<bool>{false}} {
    //
  }

  void set_value(const std::optional<std::string> &value) {
    this->m_underlying = std::expected<std::optional<std::string>, bool>{value};
  }

  [[nodiscard]] bool has_value() const {
    return this->m_underlying.has_value();
  }

  [[nodiscard]] std::optional<std::string> value() const {
    return this->m_underlying.value();
  }
};

struct CommandLineArguments {
  std::string host;
  int port;
  std::string dbname;

  std::optional<std::string> user;
  std::optional<std::string> password;

  std::string file;
  std::optional<std::string> type;
  std::optional<bool> hasHead;

  TableMode table_mode;

  LogLevel level;
  IgnoreErrors ignoreErrors;
  bool multiThreaded;
  std::optional<std::uint32_t> threads;

  std::uint64_t memorySize;
  std::optional<std::uint32_t> transactionSize;

  [[nodiscard]] bool should_print(LogLevel message_level) const;
};

namespace helper {
std::expected<CommandLineArguments, std::string>
parse_args(const std::vector<std::string> &arguments);
} // namespace helper
