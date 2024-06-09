
#include "cli_arguments.hpp"

#include <argparse/argparse.hpp>
#include <optional>

#define STRINGIFY(a) STRINGIFY_HELPER_(a)
#define STRINGIFY_HELPER_(a) #a

helper::expected<CommandLineArguments, std::string>
helper::parse_args(const std::vector<std::string> &arguments) {
  argparse::ArgumentParser parser{"imdb-sql-importer", STRINGIFY(_APP_VERSION),
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

  parser.add_argument("-p", "--password")
      .help("set the password of the PostgreSQL Server")
      .metavar("password");

  parser.add_argument("-f", "--file")
      .help("the file to import")
      .metavar("file")
      .required();

  // TODO: make this optional and detect it based on the filename
  parser.add_argument("-t", "--type")
      .help("the type to import")
      .metavar("type")
      .required();

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
      hasHead = true;
    }

    return CommandLineArguments{
        .host = parser.get<std::string>("host"),
        .port = parser.get<int>("port"),
        .dbname = parser.get<std::string>("dbname"),
        .user = parser.get<std::string>("user"),
        .password = parser.get<std::string>("password"),
        .file = parser.get<std::string>("file"),
        .type = parser.get<std::string>("type"),
        .hasHead = hasHead,
    };

  } catch (const std::exception &error) {
    return helper::unexpected<std::string>{error.what()};
  }
}
