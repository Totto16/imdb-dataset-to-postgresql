
#pragma once

#include "helper/cli_arguments.hpp"
#include "helper/expected.hpp"

#include "../postgres.hpp"
#include <type_traits>

namespace helper {

[[nodiscard]] helper::expected<postgres::Connection, std::string>
get_connection(const CommandLineArguments &arguments);

}

// from:
// https://stackoverflow.com/questions/11251376/how-can-i-check-if-a-type-is-an-instantiation-of-a-given-class-template
template <template <typename...> class Template, typename T>
struct is_instantiation_of : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct is_instantiation_of<Template, Template<Args...>> : std::true_type {};

template <typename S> using IsOptional = is_instantiation_of<std::optional, S>;

namespace {
struct TypesCollector {
  template <typename T> void accept(char const *const name) {
    (void)name;

    Oid oid;
    if constexpr (IsOptional<T>::value) {
      oid = oid_of(static_cast<T::value_type *>(nullptr));
    } else {
      oid = oid_of(static_cast<T *>(nullptr));
    }

    types.push_back(oid);
  }

  std::vector<Oid> types;

private:
  Oid oid_of(bool *) { return BOOLOID; }

  Oid oid_of(std::int8_t *) { return CHAROID; }
  Oid oid_of(std::int16_t *) { return INT2OID; }
  Oid oid_of(std::int32_t *) { return INT4OID; }
  Oid oid_of(std::int64_t *) { return INT8OID; }

  Oid oid_of(float *) { return FLOAT4OID; }
  Oid oid_of(double *) { return FLOAT8OID; }

  Oid oid_of(std::string *) { return TEXTOID; }

  Oid oid_of(std::vector<std::string> *) { return TEXTARRAYOID; }
};

} // namespace

namespace helper {

template <typename T> struct PreparedStatement {
  static std::vector<Oid> types() {
    auto collector = TypesCollector{};
    T::visitPostgresDefinition(collector);
    return collector.types;
  }
};
} // namespace helper
