

#include <iostream>
#include <vector>

#include "TSVParser.hpp"
#include "helper/cli_arguments.hpp"
#include "helper/postgres.hpp"

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
              << parsedArguments.error();
    return EXIT_FAILURE;
  }

  auto arguments = std::move(parsedArguments.value());

  auto maybeConnection = helper::get_connection(arguments);

  if (not maybeConnection.has_value()) {

    std::cerr << "error connecting to database: " << maybeConnection.error();
    return EXIT_FAILURE;
  }

  auto connection = std::move(maybeConnection.value());

  MaybeParser maybeParser =
      makeParser(arguments.file, arguments.type, arguments.hasHead);

  if (not maybeParser.has_value()) {
    std::cerr << "parser error:" << maybeParser.error();
    return EXIT_FAILURE;
  }

  auto parser = std::move(maybeParser);

  auto result = parser->parseData(connection);

  if (not result.has_value()) {
    std::cerr << "parser error:" << result.error();
    return EXIT_FAILURE;
  }

  std::cout << "Successfully inserted " << result.value() << "rows";
  return EXIT_SUCCESS;
}
