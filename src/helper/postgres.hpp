
#pragma once

#include "helper/cli_arguments.hpp"

#include <expected>

#include <postgres/Postgres.h>

namespace helper {

[[nodiscard]] std::expected<postgres::Connection, std::string>
get_connection(const CommandLineArguments &arguments);

}
