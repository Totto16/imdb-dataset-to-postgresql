

#include <iostream>
#include <sys/sysinfo.h>
#include <vector>

#include "ParseMetadata.hpp"

#include "helper/cli_arguments.hpp"

#include "helper/threads.hpp"

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
    ;
    return EXIT_FAILURE;
  }

  auto arguments = std::move(parsedArguments.value());

  ParseOptions options = {.ignoreErrors = arguments.ignoreErrors,
                          .verbose = arguments.verbose};

  helper::expected<ParseMetadata, std::string> result{};

  if (arguments.multiThreaded) {
    result = threads::multiThreadedParsers(std::move(arguments), options,
                                           get_nprocs());
  } else {
    result = threads::singleThreadedParser(std::move(arguments), options);
  }

  if (not result.has_value()) {
    std::cerr << "parser error: " << result.error() << "\n";
    std::cerr << "It took " << result->duration() << "\n";
    return EXIT_FAILURE;
  }

  std::cout << "Successfully inserted " << result->lines() << " rows\n";

  std::cout << "It took " << result->duration() << "\n";

  if (options.ignoreErrors) {
    std::cerr << "ignored " << result->errors() << " Errors\n";
  }

  return EXIT_SUCCESS;
}
