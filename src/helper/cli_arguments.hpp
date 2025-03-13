#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <vector>

enum class LogLevel : std::uint8_t { Error = 0, Warning, Info, Verbose };

struct CommandLineArguments {
  std::string host;
  int port;
  std::string dbname;

  std::optional<std::string> user;
  std::optional<std::string> password;

  std::string file;
  std::optional<std::string> type;
  std::optional<bool> hasHead;

  LogLevel level;
  bool ignoreErrors;
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
