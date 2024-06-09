#pragma once

#include "helper/expected.hpp"

#include <optional>
#include <string>
#include <vector>

struct CommandLineArguments {
  std::string host;
  int port;
  std::string dbname;

  std::optional<std::string> user;
  std::optional<std::string> password;

  std::string file;
  std::optional<std::string> type;
  std::optional<bool> hasHead;

  bool verbose;
  bool ignoreErrors;
};

namespace helper {
helper::expected<CommandLineArguments, std::string>
parse_args(const std::vector<std::string> &arguments);
} // namespace helper
