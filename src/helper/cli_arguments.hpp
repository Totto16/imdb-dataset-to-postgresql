#pragma once

#include "helper/expected.hpp"

#include <optional>
#include <string>
#include <vector>

struct CommandLineArguments {
  std::string host;
  int port;
  std::string dbname;
  std::string user;
  std::string password;

  std::string file;
  std::optional<std::string> type;
  std::optional<bool> hasHead;
};

namespace helper {
helper::expected<CommandLineArguments, std::string>
parse_args(const std::vector<std::string> &arguments);
} // namespace helper
