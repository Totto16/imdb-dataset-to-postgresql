

#include <iostream>

#include <sys/sysinfo.h>
#include <vector>

#include "ParseMetadata.hpp"
#include "helper/cli_arguments.hpp"
#include "helper/threads.hpp"
#include "print.hpp"

int main(int argc, char **argv) {

  std::vector<std::string> argumentsVector{};
  argumentsVector.reserve(argc);
  for (auto i = 0; i < argc; ++i) {
    argumentsVector.emplace_back(
        argv[i]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }

  auto parsedArguments = helper::parse_args(argumentsVector);

  if (not parsedArguments.has_value()) {

    std::cerr << "error parsing command line arguments: "
              << parsedArguments.error() << "\n";

    return EXIT_FAILURE;
  }

  auto arguments = std::move(parsedArguments.value());

  ParseOptions options = {.ignoreErrors = arguments.ignoreErrors,
                          .verbose = arguments.should_print(LogLevel::Verbose)};

  ParseResult parse_result{};

  // test connection
  { // onw scope, so that RAII cleans this connection up!
    auto maybeConnection = helper::get_connection(arguments);

    if (not maybeConnection.has_value()) {

      std::cerr << "error connecting to database: " << maybeConnection.error()
                << "\n";
      return EXIT_FAILURE;
    }

    auto &connection = maybeConnection.value();

    const auto conn_error = helper::validate_connection(connection);
    if (conn_error.has_value()) {
      std::cerr << "connection error: " << conn_error.value() << "\n";
      return EXIT_FAILURE;
    }

    const auto validate_err =
        helper::validate_tables(connection, arguments.table_mode);
    if (validate_err.has_value()) {
      std::cerr << "table validation error: " << validate_err.value() << "\n";
      return EXIT_FAILURE;
    }

    // TODO: write some values into special table
    //  e.g. version, current time, table version, postgres version etc?
  }

  if (arguments.multiThreaded) {

    std::uint32_t threads = static_cast<std::uint32_t>(get_nprocs());
    if (arguments.threads.has_value()) {
      threads = arguments.threads.value();
    }

    if (arguments.should_print(LogLevel::Info)) {
      std::cout << "Starting multi threaded import with " << threads
                << " Threads\n";
    }

    parse_result =
        threads::multiThreadedParsers(std::move(arguments), options, threads);
  } else {
    if (arguments.should_print(LogLevel::Info)) {
      std::cout << "Starting single threaded import\n";
    }

    parse_result = threads::singleThreadedParser(std::move(arguments), options);
  }

  if (not parse_result.has_value()) {
    std::cerr << "Parser error: " << parse_result.error() << "\n";
    std::cerr << "It took " << prettyPrint(parse_result.duration()) << "\n";
    return EXIT_FAILURE;
  }

  const auto result = parse_result.value();

  if (arguments.should_print(LogLevel::Info)) {

    std::cout << "Successfully inserted " << result.lines() << " rows\n";
    std::cout << "It took " << prettyPrint(parse_result.duration()) << "\n";

    if (options.ignoreErrors.has_value()) {
      std::cout << "Ignored " << result.errors() << " Errors\n";
    }
  }

  return EXIT_SUCCESS;
}

// TODO: better output, if this is used as lib, it should be possible to receive
// only necessary or all events in formatted data (e.g. json lines)
