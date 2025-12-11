
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

constexpr const char *app_version = STRINGIFY(_APP_VERSION);

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
concept IsPostgresCXXTable = requires {
  T::_POSTGRES_CXX_VISITABLE;
} && std::is_convertible_v<decltype(T::_POSTGRES_CXX_VISITABLE), bool>;

// TODO: maybe make this specifiable?
#define SCHEMA_NAME "public"

//

#define PG_CATALOG_SCHEMA "pg_catalog"

#define PG_TABLES_TABLE PG_CATALOG_SCHEMA ".pg_tables"
#define PG_TYPES_TABLE PG_CATALOG_SCHEMA ".pg_type"

#define PG_ENUM_TYPE "e"

[[nodiscard]] static bool
check_for_presence(postgres::Connection &connection,
                   const TableDescriptor &descriptor) {

  switch (descriptor.type) {
  case TableType::Table: {
    const char *query =
        "SELECT * FROM " PG_TABLES_TABLE " WHERE schemaname = '" SCHEMA_NAME
        "' AND tablename = $1;";

    const auto res = connection.exec(postgres::Command{query, descriptor.name});

    if (!res.isOk()) {
      return false;
    }

    if (res.size() == 1) {
      return true;
    }

    return false;
  }
  case TableType::Enum: {
    const char *query = "SELECT * FROM " PG_TYPES_TABLE
                        " WHERE typtype = '" PG_ENUM_TYPE "' AND typname = $1;";

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

  // TODO: make this part of the types description!

  // enums

  // taken from https://developer.imdb.com/non-commercial-datasets/
  // CREATE statements taken from my imdb_schema.sql file

  static_assert(std::is_base_of_v<postgres::Enum, pg_enums::TitleType>,
                "TitleType Must be a postgres::Enum");
  TableDescriptor ENUM_NAME_IDENT(TitleType) =
      TableDescriptor{.type = TableType::Enum,
                      .name = pg_enums::TitleType::name,
                      .dependencies = {},
                      .references = {},
                      // create enum for title_type: ATTENTION: New values may
                      // be added in the future without warning
                      .CREATE_STRING = R"(
CREATE TYPE )" SCHEMA_NAME R"(.title_basics_title_type AS ENUM (
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

  static_assert(std::is_base_of_v<postgres::Enum, pg_enums::Genre>,
                "Genre Must be a postgres::Enum");
  TableDescriptor ENUM_NAME_IDENT(Genre) =
      TableDescriptor{.type = TableType::Enum,
                      .name = pg_enums::Genre::name,
                      .dependencies = {},
                      .references = {},
                      // create enum for genres: ATTENTION: New values may be
                      // added in the future without warning
                      .CREATE_STRING = R"(
CREATE TYPE )" SCHEMA_NAME R"(.title_basics_genres AS ENUM (
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

  static_assert(std::is_base_of_v<postgres::Enum, pg_enums::GeneralJob>,
                "GeneralJobMust be a postgres::Enum");
  TableDescriptor ENUM_NAME_IDENT(GeneralJob) =
      TableDescriptor{.type = TableType::Enum,
                      .name = pg_enums::GeneralJob::name,
                      .dependencies = {},
                      .references = {},
                      // create enum for general_job: ATTENTION: New values may
                      // be added in the future without warning
                      .CREATE_STRING = R"(
CREATE TYPE )" SCHEMA_NAME R"(.general_job AS ENUM (
	'director',
	'cinematographer',
	'composer',
	'editor',
	'actor',
	'miscellaneous',
	'producer',
	'actress',
	'writer',
	'soundtrack',
	'archive_footage',
	'music_department',
	'stunts',
	'make_up_department',
	'assistant_director',
	'casting_department',
	'music_artist',
	'camera_department',
	'art_department',
	'casting_director',
	'executive',
	'costume_designer',
	'script_department',
	'art_director',
	'editorial_department',
	'costume_department',
	'animation_department',
	'talent_agent',
	'archive_sound',
	'special_effects',
	'production_manager',
	'production_designer',
	'sound_department',
	'visual_effects',
	'location_management',
	'set_decorator',
	'transportation_department',
	'choreographer',
	'manager',
	'publicist',
	'podcaster',
	'legal',
	'assistant',
	'production_department',
	'electrical_department',
	'accountant',
	'self'
);)"};

  static_assert(std::is_base_of_v<postgres::Enum, pg_enums::AlternativeType>,
                "AlternativeType Must be a postgres::Enum");
  TableDescriptor ENUM_NAME_IDENT(AlternativeType) =
      TableDescriptor{.type = TableType::Enum,
                      .name = pg_enums::AlternativeType::name,
                      .dependencies = {},
                      .references = {},
                      // create enum for type: ATTENTION: New values may be
                      // added in the future without warning
                      .CREATE_STRING = R"(
CREATE TYPE )" SCHEMA_NAME R"(.title_akas_type AS ENUM (
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

  static_assert(IsPostgresCXXTable<pg_tables::IMDBImporterMetadata>,
                "IMDBImporterMetadata needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(IMDBImporterMetadata) = {
      .type = TableType::Table,
      .name = pg_tables::IMDBImporterMetadata::_POSTGRES_CXX_TABLE_NAME,
      .dependencies = {},
      .references = {},
      // create the table imdb_importer_metadata
      .CREATE_STRING = R"(
CREATE TABLE  )" SCHEMA_NAME R"(.imdb_importer_metadata (
	id SERIAL4 NOT NULL,
	version INT4 NOT NULL,
	versionString TEXT NOT NULL,
	description TEXT NOT NULL,
	timestamp TIMESTAMP NOT NULL,
	options TEXT,
	PRIMARY KEY(id)
);)"};

  static_assert(IsPostgresCXXTable<pg_tables::TitleBasic>,
                "TitleBasic needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleBasic) = {
      .type = TableType::Table,
      .name = pg_tables::TitleBasic::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(TitleType),
              ENUM_NAME_IDENT(Genre),

          },
      .references = {},
      .CREATE_STRING = R"(
CREATE TABLE  )" SCHEMA_NAME R"(.title_basics (
	tconst TEXT NOT NULL,
	titleType title_basics_title_type NOT NULL,
	primaryTitle TEXT NOT NULL,
	originalTitle TEXT NOT NULL,
	isAdult BOOLEAN NOT NULL,
	startYear INT4,
	endYear INT4,
	runTimeMinutes INT4,
	genres title_basics_genres [],
	PRIMARY KEY(tconst)
);)"};

  static_assert(IsPostgresCXXTable<pg_tables::NameBasic>,
                "NameBasic needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(NameBasic) = {
      .type = TableType::Table,
      .name = pg_tables::NameBasic::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(GeneralJob),

          },
      .references = {},
      .CREATE_STRING = R"(
CREATE TABLE  )" SCHEMA_NAME R"(.name_basics (
	nconst TEXT NOT NULL,
	primaryName TEXT NOT NULL,
	birthYear INT4,
	deathYear INT4,
	primaryProfession general_job [] NOT NULL,
	knownForTitles TEXT [] NOT NULL,
	PRIMARY KEY (nconst)
);)"};

  static_assert(IsPostgresCXXTable<pg_tables::TitleAlternate>,
                "TitleAlternate needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleAlternate) = {
      .type = TableType::Table,
      .name = pg_tables::TitleAlternate::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(AlternativeType),

          },
      .references =
          {
              TABLE_NAME_IDENT(TitleBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE  )" SCHEMA_NAME R"(.title_akas (
	titleId TEXT NOT NULL,
	ordering INT4 NOT NULL,
	title TEXT NOT NULL,
	region TEXT NOT NULL,
	language TEXT,
	types title_akas_type [] NOT NULL,
	attributes TEXT [] NOT NULL,
	isOriginalTitle BOOLEAN NOT NULL,
	PRIMARY KEY(titleId, ordering),
	FOREIGN KEY (titleId) REFERENCES  )" SCHEMA_NAME
                       R"(.title_basics(tconst)
);)"};

  static_assert(IsPostgresCXXTable<pg_tables::TitleCrew>,
                "TitleCrew needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleCrew) = {
      .type = TableType::Table,
      .name = pg_tables::TitleCrew::_POSTGRES_CXX_TABLE_NAME,
      .dependencies = {},
      .references =
          {
              TABLE_NAME_IDENT(TitleBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE  )" SCHEMA_NAME R"(.title_crew (
	tconst TEXT NOT NULL,
	directors TEXT [] NOT NULL,
	writers TEXT [] NOT NULL,
	PRIMARY KEY(tconst),
	FOREIGN KEY (tconst) REFERENCES  )" SCHEMA_NAME R"(.title_basics(tconst)
);)"};

  static_assert(IsPostgresCXXTable<pg_tables::TitleEpisode>,
                "TitleEpisode needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleEpisode) = {
      .type = TableType::Table,
      .name = pg_tables::TitleEpisode::_POSTGRES_CXX_TABLE_NAME,
      .dependencies = {},
      .references =
          {
              TABLE_NAME_IDENT(TitleBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE  )" SCHEMA_NAME R"(.title_episode (
	tconst TEXT NOT NULL,
	parentTconst TEXT NOT NULL,
	seasonNumber INT4 NOT NULL,
	episodeNumber INT4 NOT NULL,
	PRIMARY KEY(tconst),
	FOREIGN KEY (tconst) REFERENCES  )" SCHEMA_NAME
                       R"(.title_basics(tconst),
	FOREIGN KEY (parentTconst) REFERENCES  )" SCHEMA_NAME
                       R"(.title_basics(tconst)
);)"};

  static_assert(IsPostgresCXXTable<pg_tables::TitlePrincipal>,
                "TitlePrincipal needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitlePrincipal) = {
      .type = TableType::Table,
      .name = pg_tables::TitlePrincipal::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(GeneralJob),
          },
      .references =
          {
              TABLE_NAME_IDENT(NameBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE  )" SCHEMA_NAME R"(.title_principals (
	tconst TEXT NOT NULL,
	ordering INT4 NOT NULL,
	nconst TEXT NOT NULL,
	category general_job NOT NULL,
	job TEXT,
	characters TEXT [] NOT NULL,
	PRIMARY KEY(tconst, ordering),
	FOREIGN KEY (nconst) REFERENCES  )" SCHEMA_NAME R"(.name_basics(nconst),
	FOREIGN KEY (tconst) REFERENCES  )" SCHEMA_NAME R"(.title_basics(tconst)
);)"};

  static_assert(IsPostgresCXXTable<pg_tables::TitleRating>,
                "TitleRating needs to be a postgres table");
  TableDescriptor TABLE_NAME_IDENT(TitleRating) = {
      .type = TableType::Table,
      .name = pg_tables::TitleRating::_POSTGRES_CXX_TABLE_NAME,
      .dependencies =
          {
              ENUM_NAME_IDENT(GeneralJob),
          },
      .references =
          {
              TABLE_NAME_IDENT(TitleBasic),
          },
      .CREATE_STRING = R"(
CREATE TABLE  )" SCHEMA_NAME R"(.title_ratings (
	tconst TEXT NOT NULL,
	averageRating REAL NOT NULL,
	numVotes INT4 NOT NULL,
	PRIMARY KEY(tconst),
	FOREIGN KEY (tconst) REFERENCES  )" SCHEMA_NAME R"(.title_basics(tconst)
);
)"};

  // these need to be in dependency order
  std::vector<TableDescriptor> tables = {TABLE_NAME_IDENT(IMDBImporterMetadata),
                                         TABLE_NAME_IDENT(TitleBasic),
                                         TABLE_NAME_IDENT(NameBasic),
                                         TABLE_NAME_IDENT(TitleAlternate),
                                         TABLE_NAME_IDENT(TitleCrew),
                                         TABLE_NAME_IDENT(TitleEpisode),
                                         TABLE_NAME_IDENT(TitlePrincipal),
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

    if (has_descriptor(table)) {
      continue;
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

  } catch (const postgres::RuntimeError &err) {
    return err.what();
  } catch (const std::runtime_error &err) {
    return err.what();
  }
}

// stable Version, if this is not the same in the database as here, it is not
// compatible!
constexpr static std::int64_t CURRENT_TABLE_VERSION = 2;

void helper::insert_version_marker(postgres::Connection &connection) {
  const std::int64_t version = CURRENT_TABLE_VERSION;
  const std::string versionString = app_version;
  const std::string description =
      std::string{"generated by: "} + application_name;
  const std::string options = "";

  auto res = connection.exec(postgres::Command{
      R"(INSERT INTO  )" SCHEMA_NAME
      R"(.imdb_importer_metadata (version, versionString, description, timestamp, options) VALUES ($1, $2, $3, NOW(), $4);)",
      version, versionString, description, options});

  if (!res.isOk()) {
    throw std::runtime_error{std::string{"creation failed: "} + res.message()};
  }
}
