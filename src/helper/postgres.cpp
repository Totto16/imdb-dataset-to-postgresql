
#include <expected>

#include "./postgres.hpp"
#include "./types.hpp"

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

  try {

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

    postgres::Connection connection{config};

    const auto result = postgres::Connection::ping(config);

    if (result != PGPing::PQPING_OK) {
      return std::unexpected<std::string>{pingResultToName(result)};
    }

    if (!connection.isOk()) {
      return std::unexpected<std::string>{"Connection not ok: " +
                                          connection.message()};
    }

    return connection;

  } catch (const postgres::Error &error) {
    return std::unexpected<std::string>{std::string{"postgres error: "} +
                                        error.what()};
  }
}

std::optional<std::string>
helper::validate_connection(postgres::Connection &connection) {

  try {

    const auto result = connection.execRaw("SELECT 1;");

    if (!result.isOk()) {
      return "Connection not ok: " + connection.message();
    }

    return std::nullopt;

  } catch (const postgres::Error &error) {
    return std::optional<std::string>{std::string{"postgres error: "} +
                                      error.what()};
  }
}

enum class TableType : std::uint8_t {
  Enum,
  Table,
};

#define CREATE_STRING __POSTGRES_CXX_CREATE_STRING

struct TableDescriptor {
  TableType type;
  const char *name;
  std::vector<TableDescriptor> dependencies;
  std::vector<TableDescriptor> references;

  const char *CREATE_STRING;
};

#define ENUM_NAME_IDENT(Name) Name##_ENUM

#define TABLE_NAME_IDENT(Name) Name##_TABLE

template <typename T>
concept PostgresCxxVisitable = requires {
  T::_POSTGRES_CXX_VISITABLE;
} && std::is_convertible_v<decltype(T::_POSTGRES_CXX_VISITABLE), bool>;

// TODO: maybe make this specifiable?
static constexpr const char *SCHEMA = "public";

//

#define CATALOG_SCHEMA "pg_catalog"

#define TABLES_TABLE CATALOG_SCHEMA ".pg_tables"
#define TYPES_TABLE CATALOG_SCHEMA ".pg_type"

#define PG_ENUM_TYPE "'e'"

[[nodiscard]] static bool
check_for_presence(postgres::Connection &connection,
                   const TableDescriptor &descriptor) {

  switch (descriptor.type) {
  case TableType::Table: {
    const char *query = "SELECT * FROM " TABLES_TABLE
                        " WHERE schemaname = $1 AND tablename = $2;";

    const auto res =
        connection.exec(postgres::Command{query, SCHEMA, descriptor.name});

    if (!res.isOk()) {
      return false;
    }

    if (res.size() == 1) {
      return true;
    }

    return false;
  }
  case TableType::Enum: {
    const char *query = "SELECT * FROM " TYPES_TABLE
                        " WHERE typtype = " PG_ENUM_TYPE " AND typname = $1;";

    const auto res = connection.exec(postgres::Command{query, descriptor.name});

    if (!res.isOk()) {
      return false;
    }

    if (res.size() == 1) {
      return true;
    }

    return false;
  }
  default: {
    return false;
  }
  }
}

static void create_type(postgres::Connection &connection,
                        const TableDescriptor &table_desc) {

  const auto result =
      connection.exec(postgres::Command{table_desc.CREATE_STRING});

  if (!result.isOk()) {
    throw std::runtime_error{std::string{"creation failed: "} +
                             result.message()};
  }
}

[[nodiscard]] static std::optional<std::string>
validate_all_tables(postgres::Connection &connection, bool allow_create) {

  // enums

  static_assert(std::is_base_of_v<postgres::Enum, TitleType>,
                "TitleType Must be a postgres::Enum");
  TableDescriptor ENUM_NAME_IDENT(TitleType) =
      TableDescriptor{.type = TableType::Enum,
                      .name = TitleType::name,
                      .dependencies = {},
                      .references = {},
                      .CREATE_STRING = R"(
CREATE TYPE title_basics_title_type AS ENUM (
	'movie',
	'short',
	'tvEpisode',
	'tvMiniSeries',
	'tvMovie',
	'tvSeries',
	'tvShort',
	'tvSpecial',
	'video',
	'videoGame'
);)"};

  static_assert(std::is_base_of_v<postgres::Enum, Genre>,
                "Genre Must be a postgres::Enum");
  TableDescriptor ENUM_NAME_IDENT(Genre) =
      TableDescriptor{.type = TableType::Enum,
                      .name = Genre::name,
                      .dependencies = {},
                      .references = {},
                      .CREATE_STRING = R"(
CREATE TYPE title_basics_genres AS ENUM (
	'Game-Show',
	'Family',
	'Music',
	'Reality-TV',
	'Comedy',
	'Western',
	'Short',
	'Crime',
	'War',
	'Romance',
	'Biography',
	'Drama',
	'Mystery',
	'Sci-Fi',
	'Fantasy',
	'Adventure',
	'Documentary',
	'Action',
	'Animation',
	'Sport',
	'Horror',
	'Adult',
	'News',
	'Talk-Show',
	'Film-Noir',
	'Musical',
	'Thriller',
	'History'
);)"};

  static_assert(std::is_base_of_v<postgres::Enum, GeneralJob>,
                "GeneralJobMust be a postgres::Enum");
  TableDescriptor ENUM_NAME_IDENT(GeneralJob) =
      TableDescriptor{.type = TableType::Enum,
                      .name = GeneralJob::name,
                      .dependencies = {},
                      .references = {},
                      .CREATE_STRING = R"(
CREATE TYPE general_job AS ENUM (
	'director',
	'cinematographer',
	'composer',
	'editor',
	'actor'
);)"};

  static_assert(std::is_base_of_v<postgres::Enum, AlternativeType>,
                "AlternativeType Must be a postgres::Enum");
  TableDescriptor ENUM_NAME_IDENT(AlternativeType) =
      TableDescriptor{.type = TableType::Enum,
                      .name = AlternativeType::name,
                      .dependencies = {},
                      .references = {},
                      .CREATE_STRING = R"(
CREATE TYPE title_akas_type AS ENUM (
	'alternative',
	'dvd',
	'festival',
	'tv',
	'video',
	'working',
	'original',
	'imdbDisplay'
);)"};

  // tables

  static_assert(PostgresCxxVisitable<TitleBasic>,
                "TitleBasic needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleBasic) = {
      .type = TableType::Enum,
      .name = TitleBasic::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(TitleType),
              ENUM_NAME_IDENT(Genre),

          },
      .references = {},
      .CREATE_STRING = R"(
CREATE TABLE IF NOT EXISTS public.title_basics (
	tconst TEXT NOT NULL,
	titleType title_basics_title_type NOT NULL,
	primaryTitle TEXT NOT NULL,
	originalTitle TEXT NOT NULL,
	isAdult boolean NOT NULL,
	startYear INT,
	endYear INT,
	runTimeMinutes INT,
	genres title_basics_genres [],
	PRIMARY KEY(tconst)
);)"};

  static_assert(PostgresCxxVisitable<NameBasic>,
                "NameBasic needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(NameBasic) = {
      .type = TableType::Enum,
      .name = NameBasic::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(GeneralJob),

          },
      .references = {},
      .CREATE_STRING = R"(
CREATE TABLE IF NOT EXISTS public.name_basics (
	nconst TEXT NOT NULL,
	primaryName TEXT NOT NULL,
	birthYear INT,
	deathYear INT,
	primaryProfession general_job [] NOT NULL,
	knownForTitles TEXT [] NOT NULL,
	PRIMARY KEY (nconst)
);)"};

  static_assert(PostgresCxxVisitable<TitleAlternate>,
                "TitleAlternate needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleAlternate) = {
      .type = TableType::Enum,
      .name = TitleAlternate::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(AlternativeType),

          },
      .references =
          {
              TABLE_NAME_IDENT(TitleBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE IF NOT EXISTS public.title_akas (
	titleId TEXT NOT NULL,
	ordering INT NOT NULL,
	title TEXT NOT NULL,
	region TEXT NOT NULL,
	language TEXT,
	types title_akas_type [] NOT NULL,
	attributes TEXT [] NOT NULL,
	isOriginalTitle BOOLEAN NOT NULL,
	PRIMARY KEY(titleId, ordering),
	FOREIGN KEY (titleId) REFERENCES public.title_basics(tconst)
);)"};

  static_assert(PostgresCxxVisitable<TitleCrew>,
                "TitleCrew needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleCrew) = {
      .type = TableType::Enum,
      .name = TitleCrew::_POSTGRES_CXX_TABLE_NAME,
      .dependencies = {},
      .references =
          {
              TABLE_NAME_IDENT(TitleBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE IF NOT EXISTS public.title_crew (
	tconst TEXT NOT NULL,
	directors TEXT [] NOT NULL,
	writers TEXT [] NOT NULL,
	PRIMARY KEY(tconst),
	FOREIGN KEY (tconst) REFERENCES public.title_basics(tconst)
);)"};

  static_assert(PostgresCxxVisitable<TitleEpisode>,
                "TitleEpisode needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleEpisode) = {
      .type = TableType::Enum,
      .name = TitleEpisode::_POSTGRES_CXX_TABLE_NAME,
      .dependencies = {},
      .references =
          {
              TABLE_NAME_IDENT(TitleBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE IF NOT EXISTS public.title_episode (
	tconst TEXT NOT NULL,
	parentTconst TEXT NOT NULL,
	seasonNumber INT NOT NULL,
	episodeNumber INT NOT NULL,
	PRIMARY KEY(tconst),
	FOREIGN KEY (tconst) REFERENCES public.title_basics(tconst),
	FOREIGN KEY (parentTconst) REFERENCES public.title_basics(tconst)
);)"};

  static_assert(PostgresCxxVisitable<TitlePrincipal>,
                "TitlePrincipal needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitlePrincipal) = {
      .type = TableType::Enum,
      .name = TitlePrincipal::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(GeneralJob),
          },
      .references =
          {
              TABLE_NAME_IDENT(NameBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE IF NOT EXISTS public.title_principals (
	tconst TEXT NOT NULL,
	ordering INT NOT NULL,
	nconst TEXT NOT NULL,
	category general_job NOT NULL,
	job TEXT,
	characters TEXT [],
	PRIMARY KEY(tconst, ordering),
	FOREIGN KEY (nconst) REFERENCES public.name_basics(nconst),
	FOREIGN KEY (tconst) REFERENCES public.title_basics(tconst)
);)"};

  static_assert(PostgresCxxVisitable<TitleRating>,
                "TitleRating needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleRating) = {
      .type = TableType::Enum,
      .name = TitleRating::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(GeneralJob),
          },
      .references =
          {
              TABLE_NAME_IDENT(TitleBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE IF NOT EXISTS public.title_ratings (
	tconst TEXT NOT NULL,
	averageRating NUMERIC NOT NULL,
	numVotes INT NOT NULL,
	PRIMARY KEY(tconst),
	FOREIGN KEY (tconst) REFERENCES public.title_basics(tconst)
);
)"};

  // these need to be in dependency order
  std::vector<TableDescriptor> tables = {
      TABLE_NAME_IDENT(TitleBasic),     TABLE_NAME_IDENT(NameBasic),
      TABLE_NAME_IDENT(TitleAlternate), TABLE_NAME_IDENT(TitleCrew),
      TABLE_NAME_IDENT(TitleEpisode),   TABLE_NAME_IDENT(TitlePrincipal),
      TABLE_NAME_IDENT(TitleRating)};

  std::vector<TableDescriptor> validated = {};

  const auto has_descriptor =
      [&validated](const TableDescriptor &desc) -> bool {
    if (validated.empty()) {
      return false;
    }

    auto it = std::find_if(validated.begin(), validated.end(),
                           [desc](const TableDescriptor &t2) -> bool {
                             return desc.name == t2.name;
                           });

    return (it != validated.end());
  };

  const auto is_not_present_handling =
      [&connection, allow_create](const TableDescriptor &desc) -> void {
    if (!allow_create) {
      throw std::runtime_error{std::string{"Type "} + desc.name +
                               " no present"};
    }

    create_type(connection, desc);
  };

  for (const auto &table : tables) {

    for (const auto &dependency : table.dependencies) {

      if (has_descriptor(dependency)) {
        continue;
      }

      const auto is_present = check_for_presence(connection, dependency);

      if (is_present) {
        validated.push_back(dependency);
      } else {
        is_not_present_handling(dependency);
      }
    }

    for (const auto &reference : table.references) {

      if (has_descriptor(reference)) {
        continue;
      }

      const auto is_present = check_for_presence(connection, reference);

      if (is_present) {
        validated.push_back(reference);
      } else {
        is_not_present_handling(reference);
      }
    }

    const auto is_present = check_for_presence(connection, table);
    if (is_present) {
      validated.push_back(table);
    } else {
      is_not_present_handling(table);
    }
  }

  return std::nullopt;
}

std::optional<std::string>
helper::validate_tables(postgres::Connection &connection,
                        TableMode table_mode) {

  try {

    switch (table_mode) {
    case TableMode::DoNohing: {
      return std::nullopt;
    case TableMode::Check: {
      return validate_all_tables(connection, false);
    }
    case TableMode::CheckAndCreate: {
      return validate_all_tables(connection, true);
    }
    default:
      return "Error: invalid TableMode enum";
    }
    }

  } catch (const std::runtime_error &err) {
    return err.what();
  }
}
