
#include <errno.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "InternalParsers.hpp"

[[nodiscard]] bool StaticParsers::isNulledValue(const std::string &str) {
  return ("\\N" == str);
}

[[nodiscard]] pg_types::Text StaticParsers::asIs(const std::string &str) {
  return str;
}

[[nodiscard]] pg_types::Nullable<pg_types::Text>
StaticParsers::asIsNullable(const std::string &str) {

  if (StaticParsers::isNulledValue(str)) {
    return std::nullopt;
  }

  return str;
}

[[nodiscard]] pg_types::Bool
StaticParsers::booleanParser(const std::string &str) {
  if (str == "0") {
    return false;
  } else if (str == "1") {
    return true;
  } else {
    throw std::runtime_error("Couldn't parse boolean of '" + str + "'");
  }
}

enum class STR2DOUBLE_ERROR {
  SUCCESS,
  OUT_OF_RANGE,
  INCONVERTIBLE,
};

static STR2DOUBLE_ERROR str2double(double &d, char const *s) {
  size_t pos;
  try {
    d = std::stod(s, &pos);
  } catch (const std::invalid_argument &ex) {
    return STR2DOUBLE_ERROR::INCONVERTIBLE;
  } catch (const std::out_of_range &ex) {
    return STR2DOUBLE_ERROR::OUT_OF_RANGE;
  }

  if (pos != strlen(s)) {
    return STR2DOUBLE_ERROR::INCONVERTIBLE;
  }
  return STR2DOUBLE_ERROR::SUCCESS;
}

[[nodiscard]] pg_types::DoublePrecision
StaticParsers::doubleParser(const std::string &str) {
  double d;
  const auto result = str2double(d, str.c_str());
  if (result != STR2DOUBLE_ERROR::SUCCESS) {
    throw std::runtime_error("Couldn't parse double of '" + str + "'");
  }

  return static_cast<pg_types::DoublePrecision>(d);
}

[[nodiscard]] pg_types::Real
StaticParsers::floatParser(const std::string &str) {
  double d = StaticParsers::doubleParser(str);
  return static_cast<pg_types::Real>(d);
}
