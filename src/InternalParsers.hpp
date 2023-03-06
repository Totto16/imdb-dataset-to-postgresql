
#pragma once

#include "constructable.hpp"

#include <functional>
#include <memory>
#include <nan.h>
#include <string>

using namespace std;
using namespace v8;

class ParserExceptionConstructable : public Constructable {
public:
  explicit ParserExceptionConstructable(const std::string &value)
      : Constructable(), m_value{std::move(value)} {};

  virtual v8::Local<v8::Value> construct(Nan::HandleScope &scope,
                                         v8::Isolate *isolate) const override {
    // TODO make some mechanimsm to get the exception out as aerly as possible!
    return Nan::TypeError(m_value.c_str());
  };

private:
  std::string m_value;
};

using ParserFunction = function<shared_ptr<Constructable>(const string &str)>;

class StaticParsers {
public:
  [[nodiscard]] static bool isNulledValue(const string &str);

  [[nodiscard]] static shared_ptr<Constructable> asIs(const string &str);

  [[nodiscard]] static shared_ptr<Constructable>
  imdbIdParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  alternativeTitleParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  titleTypeParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable> genreParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  nameIDParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  regionParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  languageParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable>
  booleanParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable> floatParser(const string &str);
  [[nodiscard]] static shared_ptr<Constructable> intParser(const string &str);

  [[nodiscard]] static ParserFunction orNullParser(const ParserFunction &fn);
  [[nodiscard]] static ParserFunction arrayParser(const ParserFunction &fn);

}; // namespace Parser
