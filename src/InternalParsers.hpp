
#pragma once

#include <nan.h>
#include <string>

using namespace std;
using namespace v8;

class Parser {
public:
  [[nodiscard]] static bool isNulledValue(const string &str);

  [[nodiscard]] static Local<Value> asIs(const string &str);

  [[nodiscard]] static Local<Value> imdbIdParser(const string &str);
  [[nodiscard]] static Local<Value> alternativeTitleParser(const string &str);
  [[nodiscard]] static Local<Value> titleTypeParser(const string &str);
  [[nodiscard]] static Local<Value> genreParser(const string &str);
  [[nodiscard]] static Local<Value> nameIDParser(const string &str);

}; // namespace Parser

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