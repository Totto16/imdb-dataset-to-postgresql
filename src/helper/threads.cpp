
#include "threads.hpp"
#include "ParseMetadata.hpp"
#include "TSVParser.hpp"
#include "helper/postgres.hpp"
#include "postgres/Connection.h"

#include <BS_thread_pool.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sys/stat.h>

namespace {
size_t getFilesize(const char *filename) {
  struct stat st {};
  ::stat(filename, &st);
  return st.st_size;
}

std::tuple<std::vector<std::size_t>, std::size_t, std::uint64_t>
getFileChunksByNewLine(const std::filesystem::path &file,
                       std::size_t desiredChunkSize, std::size_t nproc) {

  std::ifstream fileStream{file, std::ios::binary};
  auto iterator = std::istreambuf_iterator<char>{fileStream};

  std::istreambuf_iterator<char> end{};

  const auto fileSize = getFilesize(file.c_str());

  auto usedChunkSize = desiredChunkSize;

  if (fileSize / desiredChunkSize < nproc) {
    usedChunkSize = (fileSize / nproc) + 1;
  }

  std::vector<std::size_t> result{0};
  result.reserve((fileSize / usedChunkSize) + 1);

  std::uint64_t lines = 0;

  std::size_t i = 1;
  for (std::size_t currentChunkSize = 1; iterator != end; ++iterator) {
    const char value = *iterator;

    if (value == '\n') {
      ++lines;
      if (currentChunkSize >= usedChunkSize) {
        result.push_back(i);
        currentChunkSize = 0;
      }
    }

    ++i;
    ++currentChunkSize;
  }

  result.push_back(i);

  return {result, usedChunkSize, lines};
}

} // namespace

ParseResult threads::multiThreadedParsers(CommandLineArguments &&_arguments,
                                          const ParseOptions &options,
                                          int nproc) {

  const auto arguments = std::move(_arguments);

  if (!std::filesystem::exists(arguments.file)) {
    std::cerr << "File doesn't exist\n";
    std::exit(EXIT_FAILURE);
  }

  // TODO: make customizable
  auto desiredChunkSize = 256 * 1024 * 1024 / nproc; // 256 MB / nproc

  auto [chunks, _, lineAmount] = getFileChunksByNewLine(
      arguments.file, desiredChunkSize, static_cast<std::uint32_t>(nproc));

  BS::thread_pool pool{static_cast<std::uint32_t>(nproc)};

  std::shared_ptr<ParseMetadata> finalResult =
      std::make_shared<ParseMetadata>();
  std::shared_ptr<std::mutex> resultLock = std::make_shared<std::mutex>();

  for (std::size_t i = 1; i < chunks.size(); ++i) {

    const auto start = chunks.at(i - 1);
    const auto end = chunks.at(i);
    const auto length = end - start;

    pool.detach_task([arguments, options, start, length, finalResult,
                      resultLock, lineAmount] -> void {
      auto maybeConnection = helper::get_connection(arguments);

      if (not maybeConnection.has_value()) {

        std::cerr << "error connecting to database on thread : "
                  << maybeConnection.error() << "\n";
        std::exit(EXIT_FAILURE);
      }

      auto connection = std::move(maybeConnection.value());

      MaybeParser maybeParser =
          makeParser(arguments.file, arguments.type,
                     start == 0 ? arguments.hasHead : false, start, length);

      if (not maybeParser.has_value()) {
        std::cerr << "parser error: " << maybeParser.error() << "\n";
        std::exit(EXIT_FAILURE);
      }

      auto parser = std::move(maybeParser);

      auto result = parser->parseData(connection, options);

      {
        std::lock_guard<std::mutex> lock(*resultLock);

        if (not result.has_value()) {
          std::cerr << "parser error: " << result.error() << "\n";
          std::cerr << "It took " << result->duration() << "\n";
          std::exit(EXIT_FAILURE);
        }

        finalResult->addErrors(result->errors());
        finalResult->addLines(result->lines());

        const double progress =
            finalResult->lines() / static_cast<double>(lineAmount);

        std::cout << "Progress: " << (progress * 100.0) << " %\n";
      }
    });
  }

  pool.wait();

  return *finalResult;
}

ParseResult threads::singleThreadedParser(CommandLineArguments &&arguments,
                                          const ParseOptions &options) {
  auto maybeConnection = helper::get_connection(arguments);

  if (not maybeConnection.has_value()) {

    std::cerr << "error connecting to database: " << maybeConnection.error()
              << "\n";
    std::exit(EXIT_FAILURE);
  }

  auto connection = std::move(maybeConnection.value());

  MaybeParser maybeParser =
      makeParser(arguments.file, arguments.type, arguments.hasHead);

  if (not maybeParser.has_value()) {
    std::cerr << "parser error: " << maybeParser.error() << "\n";
    std::exit(EXIT_FAILURE);
  }

  auto parser = std::move(maybeParser);

  auto result = parser->parseData(connection, options);
  return result;
}
