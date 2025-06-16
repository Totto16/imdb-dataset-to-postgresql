

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

  std::expected<ParseMetadata, std::string> result{};

  if (arguments.multiThreaded) {

    std::uint32_t threads = static_cast<std::uint32_t>(get_nprocs());
    if (arguments.threads.has_value()) {
      threads = arguments.threads.value();
    }

    if (arguments.should_print(LogLevel::Info)) {
      std::cout << "Starting multi threaded import with " << threads
                << " Threads\n";
    }

    result =
        threads::multiThreadedParsers(std::move(arguments), options, threads);
  } else {
    if (arguments.should_print(LogLevel::Info)) {
      std::cout << "Starting single threaded import\n";
    }

    result = threads::singleThreadedParser(std::move(arguments), options);
  }

  if (not result.has_value()) {
    std::cerr << "Parser error: " << result.error() << "\n";
    std::cerr << "It took " << prettyPrint(result->duration()) << "\n";
    return EXIT_FAILURE;
  }

  if (arguments.should_print(LogLevel::Info)) {

    std::cout << "Successfully inserted " << result->lines() << " rows\n";
    std::cout << "It took " << prettyPrint(result->duration()) << "\n";

    if (options.ignoreErrors) {
      std::cout << "Ignored " << result->errors() << " Errors\n";
    }
  }

  return EXIT_SUCCESS;
}

// TODO: better output, if this is used as lib, it should be possible to receive
// only necessary or all events in formatted data (e.g. json lines)
