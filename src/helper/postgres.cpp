
#include <expected>

#include "./postgres.hpp"

namespace {

std::string pingResultToName(const PGPing input) {
  switch (input) {
  case PGPing::PQPING_OK:
    return "server is accepting connections";
  case PGPing::PQPING_REJECT:
    return "server is alive but rejecting connections";
  case PGPing::PQPING_NO_RESPONSE:
    return "could not establish connection";
  case PGPing::PQPING_NO_ATTEMPT:
    return "connection not attempted (bad params)";
  default:
    std::unreachable();
  }
}

#define STRINGIFY(a) STRINGIFY_HELPER_(a)
#define STRINGIFY_HELPER_(a) #a

constexpr const char *application_name =
    STRINGIFY(_APP_NAME) " v" STRINGIFY(_APP_VERSION);

#undef STRINGIFY
#undef STRINGIFY_HELPER_

} // namespace

[[nodiscard]] std::expected<postgres::Connection, std::string>
helper::get_connection(const CommandLineArguments &arguments) {

  postgres::Config::Builder builder{};

  builder.host(arguments.host);
  builder.port(arguments.port);
  builder.dbname(arguments.dbname);
  builder.application_name(application_name);

  if (arguments.user.has_value()) {
    builder.user(arguments.user.value());
  }

  if (arguments.password.has_value()) {
    builder.password(arguments.password.value());
  }

  postgres::Config config{builder.build()};

  const auto result = postgres::Connection::ping(config);

  if (result != PGPing::PQPING_OK) {
    return std::unexpected<std::string>{pingResultToName(result)};
  }

  postgres::Connection connection{config};

  return connection;
}
