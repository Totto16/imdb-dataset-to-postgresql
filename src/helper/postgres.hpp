
#pragma once

#include "helper/cli_arguments.hpp"

#include <expected>

#include <postgres/Postgres.h>

namespace helper {

[[nodiscard]] std::expected<postgres::Connection, std::string>
get_connection(const CommandLineArguments &arguments);

[[nodiscard]] std::optional<std::string>
validate_connection(postgres::Connection &connection);

[[nodiscard]] std::optional<std::string>
validate_tables(postgres::Connection &connection, TableMode table_mode);

} // namespace helper
