
#pragma once

#include "helper/cli_arguments.hpp"
#include "helper/expected.hpp"

#include "../postgres.hpp"

namespace helper {

[[nodiscard]] helper::expected<postgres::Connection, std::string>
get_connection(const CommandLineArguments &arguments);

}
