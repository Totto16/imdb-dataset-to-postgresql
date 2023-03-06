
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <array>
#include <memory>
#include <nan.h>
#include <stdexcept>

#include <string>

#include "InternalParsers.hpp"
#include "ParserStructure.hpp"
#include "constructable.hpp"

using namespace std;
using namespace v8;

[[nodiscard]] inline bool Parser::isNulledValue(const string &str) {
  return ("\\N" == str);
}

[[nodiscard]] inline shared_ptr<Constructable> Parser::asIs(const string &str) {

  if (Parser::isNulledValue(str)) {
    return make_shared<ParserExceptionConstructable>(
        "NOT nullable type was null!");
  }

  return make_shared<StringConstructable>(str);
}

[[nodiscard]] inline shared_ptr<Constructable>
Parser::imdbIdParser(const string &str) {
  return Parser::asIs(str);
}
[[nodiscard]] inline shared_ptr<Constructable>
Parser::alternativeTitleParser(const string &str) {
  return Parser::asIs(str);
}
[[nodiscard]] inline shared_ptr<Constructable>
Parser::titleTypeParser(const string &str) {
  return Parser::asIs(str);
}
[[nodiscard]] inline shared_ptr<Constructable>
Parser::genreParser(const string &str) {
  return Parser::asIs(str);
}
[[nodiscard]] inline shared_ptr<Constructable>
Parser::regionParser(const string &str) {
  return Parser::asIs(str);
}
[[nodiscard]] inline shared_ptr<Constructable>
Parser::languageParser(const string &str) {
  return Parser::asIs(str);
}

[[nodiscard]] inline shared_ptr<Constructable>
Parser::booleanParser(const string &str) {
  if (str == "0") {
    return make_shared<FalseConstructable>();
  } else if (str == "1") {
    return make_shared<TrueConstructable>();
  } else {
    return make_shared<ParserExceptionConstructable>(
        "Couldn't parse boolean of '" + str + "'");
  }
}

enum class STR2DOUBLE_ERROR { SUCCESS, OUT_OF_RANGE, INCONVERTIBLE };

STR2DOUBLE_ERROR str2double(double &d, char const *s) {
  size_t pos;
  try {
    d = std::stod(s, &pos);
  } catch (std::invalid_argument &ex) {
    return STR2DOUBLE_ERROR::INCONVERTIBLE;
  } catch (std::out_of_range &ex) {
    return STR2DOUBLE_ERROR::OUT_OF_RANGE;
  }

  if (pos != strlen(s)) {
    return STR2DOUBLE_ERROR::INCONVERTIBLE;
  }
  return STR2DOUBLE_ERROR::SUCCESS;
}

[[nodiscard]] inline shared_ptr<Constructable>
Parser::floatParser(const string &str) {
  double d;
  const auto result = str2double(d, str.c_str());
  if (result != STR2DOUBLE_ERROR::SUCCESS) {
    return make_shared<ParserExceptionConstructable>(
        "Couldn't parse float of '" + str + "'");
  }

  return make_shared<DoubleNumberConstructable>(d);
}

// from
// https://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c
enum class STR2INT_ERROR { SUCCESS, OVERFLOW, UNDERFLOW, INCONVERTIBLE };

// these ints are int32_t, since JS can only handles integers safely until ~57
// bits, so it has to be an 32 bit number to be safe!
STR2INT_ERROR str2int(int32_t &i, char const *s, int base = 10) {
  char *end;
  long l;
  errno = 0;
  // not using std::strtol, since that throws exceptions!
  l = strtol(s, &end, base);
  if ((errno == ERANGE && l == LONG_MAX) || l > INT32_MAX) {
    return STR2INT_ERROR::OVERFLOW;
  }
  if ((errno == ERANGE && l == LONG_MIN) || l < INT32_MIN) {
    return STR2INT_ERROR::UNDERFLOW;
  }
  if (*s == '\0' || *end != '\0') {
    return STR2INT_ERROR::INCONVERTIBLE;
  }
  i = (int32_t)l;
  return STR2INT_ERROR::SUCCESS;
}

[[nodiscard]] inline shared_ptr<Constructable>
Parser::intParser(const string &str) {
  int i;
  const auto result = str2int(i, str.c_str());
  if (result != STR2INT_ERROR::SUCCESS) {
    return make_shared<ParserExceptionConstructable>("Couldn't parse int of '" +
                                                     str + "'");
  }

  return make_shared<IntNumberConstructable>(i);
}

[[nodiscard]] inline ParserFunction
Parser::orNullParser(const ParserFunction &fn) {

  return [&fn](const string &str) -> shared_ptr<Constructable> {
    if (Parser::isNulledValue(str)) {
      return make_shared<NullConstructable>();
    }

    return fn(str);
  };
}

[[nodiscard]] inline ParserFunction
Parser::arrayParser(const ParserFunction &fn) {

  return [&fn](const string &str) -> shared_ptr<Constructable> {
    ArrayValues vec = ArrayValues{};

    if (Parser::isNulledValue(str)) {
      return make_shared<ArrayConstructable>(vec);
    }

    // let parts = str.split(",").filter((a) = > a !== "")
    // return parts.map(parser)
    return make_shared<ArrayConstructable>(vec);
  };
}