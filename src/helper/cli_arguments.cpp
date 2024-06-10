
#include "cli_arguments.hpp"

#include <argparse/argparse.hpp>
#include <optional>
#include <string.h>

namespace {

template <typename T>
inline std::optional<T> get_optional(argparse::ArgumentParser &parser,
                                     const std::string &value) {
  if (parser.present<T>(value)) {
    return parser.get<T>(value);
  }

  return std::nullopt;
}

constexpr const std::uint64_t defaultMemorySize = 1UL << 28; // 256 MB

static const char *human_readable_suffix = "kMGT";

// from
// https://stackoverflow.com/questions/16107613/parse-human-readable-sizes-k-m-g-t-into-bytes-in-c
std::optional<std::uint64_t> getMemorySizeImpl(char *input) {
  char *endp = input;
  const char *match = nullptr;
  size_t shift = 0;
  errno = 0;

  long double value = strtold(input, &endp);
  if (errno || endp == input || value < 0) {
    return std::nullopt;
  }

  if (!(match = strchr(human_readable_suffix, *endp))) {
    return std::nullopt;
  }

  if (*match)
    shift = (match - human_readable_suffix + 1) * 10;

  std::uint64_t target = value * (1LU << shift);

  return target;
}

std::optional<std::uint64_t> getMemorySize(const std::string &str) {
  const auto size = str.size();
  char *value = new char[size + 1];

  memcpy(value, str.c_str(), size);
  value[size] = 0;

  const auto result = getMemorySizeImpl(value);

  delete[] value;

  return result;
}

} // namespace

helper::expected<CommandLineArguments, std::string>
helper::parse_args(const std::vector<std::string> &arguments) {

#define STRINGIFY(a) STRINGIFY_HELPER_(a)
#define STRINGIFY_HELPER_(a) #a

  argparse::ArgumentParser parser{STRINGIFY(_APP_NAME), STRINGIFY(_APP_VERSION),
                                  argparse::default_arguments::all};

#undef STRINGIFY
#undef STRINGIFY_HELPER_

  parser.add_argument("-h", "--host")
      .help("set the host of the PostgreSQL Server")
      .default_value(std::string{"localhost"})
      .metavar("host");

  parser.add_argument("-p", "--port")
      .help("set the port of the PostgreSQL Server")
      .default_value(5432)
      .scan<'i', int>()
      .metavar("port");

  parser.add_argument("-d", "--dbname")
      .help("set the dbname of the PostgreSQL Server")
      .default_value(std::string{"imdb"})
      .metavar("dbname");

  parser.add_argument("-u", "--user")
      .help("set the user of the PostgreSQL Server")
      .metavar("user");

  parser.add_argument("--password")
      .help("set the password of the PostgreSQL Server")
      .metavar("password");

  parser.add_argument("-f", "--file")
      .help("the file to import")
      .metavar("file")
      .required();

  parser.add_argument("-t", "--type")
      .help("the type to import")
      .metavar("type");

  parser.add_argument("-v", "--verbose")
      .help("be more verbose about what's happening")
      .flag();

  parser.add_argument("-i", "--ignore-errors")
      .help("ignore errors and just log them")
      .flag();

  parser.add_argument("-s", "--single-threaded")
      .help("just use one thread to process the file")
      .flag();

  parser.add_argument("--threads")
      .help("how many threads to use")
      .metavar("threads")
      .scan<'u', std::uint32_t>();

  parser.add_argument("-m", "--memory-size")
      .help("the memory size to use")
      .metavar("memory-size");

  parser.add_argument("--transaction-size")
      .help("set the amount of executions per transaction")
      .scan<'u', std::uint32_t>()
      .metavar("transaction-size");

  auto &head_group = parser.add_mutually_exclusive_group();

  head_group.add_argument("--has-head")
      .help("specify that the file has a head")
      .flag();

  head_group.add_argument("--has-no-head")
      .help("specify that the file has NO head")
      .flag();

  try {
    parser.parse_args(arguments);

    std::optional<bool> hasHead = std::nullopt;

    if (parser.get<bool>("has-head")) {
      hasHead = true;
    } else if (parser.get<bool>("has-no-head")) {
      hasHead = false;
    }

    std::optional<std::string> type = std::nullopt;
    if (parser.present<std::string>("type")) {
      type = parser.get<std::string>("type");
    }

    std::uint64_t memorySize = defaultMemorySize;
    if (parser.present<std::string>("memory-size")) {
      std::string memorySizeStr = parser.get<std::string>("memory-size");
      const auto result = getMemorySize(memorySizeStr);
      if (not result.has_value()) {
        return helper::unexpected<std::string>{"Invalid memorySize option: '" +
                                               memorySizeStr + "'"};
      }
      memorySize = result.value();
    }

    return CommandLineArguments{
        .host = parser.get<std::string>("host"),
        .port = parser.get<int>("port"),
        .dbname = parser.get<std::string>("dbname"),
        .user = get_optional<std::string>(parser, "user"),
        .password = get_optional<std::string>(parser, "password"),
        .file = parser.get<std::string>("file"),
        .type = type,
        .hasHead = hasHead,
        .verbose = parser.get<bool>("verbose"),
        .ignoreErrors = parser.get<bool>("ignore-errors"),
        .multiThreaded = !parser.get<bool>("single-threaded"),
        .threads = get_optional<std::uint32_t>(parser, "threads"),
        .memorySize = memorySize,
        .transactionSize =
            get_optional<std::uint32_t>(parser, "transaction-size"),
    };

  } catch (const std::exception &error) {
    return helper::unexpected<std::string>{error.what()};
  }
}
