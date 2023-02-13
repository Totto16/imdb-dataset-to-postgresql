
#include <array>
#include <nan.h>
#include <string>

#include "InternalParsers.hpp"
#include "ParserStructure.hpp"

using namespace std;
using namespace v8;

[[nodiscard]] inline bool Parser::isNulledValue(const string &str) {
  return ("\\N" == str);
}

[[nodiscard]] inline Local<Value> Parser::asIs(const string &str) {

  if (isNulledValue(str)) {
    Nan::ThrowError("NOT nullable type was null!");
  }

  Nan::MaybeLocal<v8::String> result = Nan::New<v8::String>(str);

  return result.ToLocalChecked();
}

[[nodiscard]] inline Local<Value> Parser::imdbIdParser(const string &str) {
  return asIs(str);
}
[[nodiscard]] inline Local<Value>
Parser::alternativeTitleParser(const string &str) {
  return asIs(str);
}
[[nodiscard]] inline Local<Value> Parser::titleTypeParser(const string &str) {
  return asIs(str);
}
[[nodiscard]] inline Local<Value> Parser::genreParser(const string &str) {
  return asIs(str);
}
[[nodiscard]] inline Local<Value> Parser::nameIDParser(const string &str) {
  return asIs(str);
}

/*



export function booleanParser(str : string) : boolean {
  if (str == = "0") {
    return false
  } else if (str == = "1") {
    return true
  } else {
        throw new Error(`Couldn't parse boolean of '${
      str}'`)
  }
}

export function floatParser(str : string) : Float {
  const float = parseFloat(str) if (isNaN(float)) {
        throw new Error(`Couldn't parse float of '${
      str}'`)
  }
  return float
}

export function intParser(str : string) : Int {
  const int = parseInt(str) if (isNaN(int)) {
        throw new Error(`Couldn't parse int of '${
      str}'`)
  }
  return int
}

export function regionParser(str : string) : RegionString {
  // TODO validate
  return str as unknown as RegionString
}

export function languageParser(str : string) : LanguageString {
  // TODO validate
  return str as unknown as LanguageString
}

export function arrayParser<T>(parser : Parser<T>) : Parser<Array<T>> {
  return (str : string) : Array<T> = > {
        if (isNulledValue(str)) {
          return [] as T[]
        }

        let parts =
            str.split(",").filter((a) = > a != = "") return parts.map(parser)
  }

 v8::Local<v8::Array> results = Nan::New<v8::Array>(vec.size());
  int i = 0;
  for_each(vec.begin(), vec.end(), [&](int value) {
    Nan::Set(results, i, Nan::New<v8::Number>(value));
    i++;
  });


}

export function orNullParser<T>(parser : Parser<T>) : Parser<T | null> {
  return (str : string) : T | null = > {
        if (isNulledValue(str)) {
          return null
        }

        return parser(str)
  }
}

 */