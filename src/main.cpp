

#include <iostream>
#include <sstream>
#include <sys/sysinfo.h>
#include <vector>

#include "ParseMetadata.hpp"

#include "helper/cli_arguments.hpp"

#include "helper/threads.hpp"

namespace {
// From: https://stackoverflow.com/questions/22063979/elegant-time-print-in-c11
void prettyPrintImpl(std::ostream &out,
                     const std::chrono::milliseconds &input) {
  auto diff = input;
  auto const msecs = diff % 1000;
  diff /= 1000;
  auto const secs = diff % 60;
  diff /= 60;
  auto const mins = diff % 60;
  diff /= 60;
  auto const hours = diff % 24;
  diff /= 24;
  auto const days = diff;

  bool printed_earlier = false;
  if (days.count() >= 1) {
    printed_earlier = true;
    out << days.count() << (1 != days.count() ? " days" : " day") << ' ';
  }
  if (printed_earlier || hours.count() >= 1) {
    printed_earlier = true;
    out << hours.count() << (1 != hours.count() ? " hours" : " hour") << ' ';
  }
  if (printed_earlier || mins.count() >= 1) {
    printed_earlier = true;
    out << mins.count() << (1 != mins.count() ? " minutes" : " minute") << ' ';
  }
  if (printed_earlier || secs.count() >= 1) {
    printed_earlier = true;
    out << secs.count() << (1 != secs.count() ? " seconds" : " second") << ' ';
  }
  if (printed_earlier || msecs.count() >= 1) {
    printed_earlier = true;
    out << msecs.count()
        << (1 != msecs.count() ? " milliseconds" : " millisecond");
  }
}

std::string prettyPrint(const std::chrono::milliseconds &diff) {
  std::stringstream str{};
  prettyPrintImpl(str, diff);
  return str.str();
}

} // namespace

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

    std::uint32_t threads = static_cast<std::uint32_t>(get_nprocs());
    if (arguments.threads.has_value()) {
      threads = arguments.threads.value();
    }

    result = threads::multiThreadedParsers(std::move(arguments), options,
                                           arguments.memorySize, threads);
  } else {
    result = threads::singleThreadedParser(std::move(arguments), options);
  }

  if (not result.has_value()) {
    std::cerr << "Parser error: " << result.error() << "\n";
    std::cerr << "It took " << prettyPrint(result->duration()) << "\n";
    return EXIT_FAILURE;
  }

  std::cout << "Successfully inserted " << result->lines() << " rows\n";

  std::cout << "It took " << prettyPrint(result->duration()) << "\n";

  if (options.ignoreErrors) {
    std::cerr << "Ignored " << result->errors() << " Errors\n";
  }

  return EXIT_SUCCESS;
}
