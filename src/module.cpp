#include <expected.hpp>
#include <functional>
#include <iostream>
#include <nan.h>

#include "TSVParser.hpp"

// inspired by https://github.com/freezer333/cppwebify-tutorial

using namespace Nan;
using namespace std;
using namespace v8;

// template <typename T>

class ParseWorker : public AsyncWorker {
public:
  ParseWorker(Callback *callback, std::string path, std::string type)
      : AsyncWorker(callback), path(path), type(type) {}

  ~ParseWorker() {}

  void Execute() {
    MaybeParser maybeParser = makeParser(path, type);

    if (!maybeParser.has_value()) {
      result = tl::make_unexpected(maybeParser.error());
    } else {
      auto parser = maybeParser.value();
      result = std::move(parser.parseData());
    }
  }

  // We have the results, and we're back in the event loop.
  void HandleOKCallback() {
    Nan::HandleScope scope;

    if (result.has_value()) {

      auto resultArray = result.value();
      Local<Value> okResult[] = {Null(), resultArray};

      callback->Call(2, okResult, async_resource);
    } else {

      auto value = result.error();
      Nan::MaybeLocal<v8::String> stringValue = Nan::New<v8::String>(value);

      Local<Value> errorResult[] = {stringValue.ToLocalChecked(), Null()};

      callback->Call(2, errorResult, async_resource);
    }
  }

private:
  std::string path;
  std::string type;
  ParseResult result;
};

// Asynchronous access to the `getPrimes()` function
NAN_METHOD(parseFile) {

  if (info.Length() == 0) {
    Nan::ThrowError("You must pass a first argument");
    return;
  }

  if (!info[0]->IsString()) {
    Nan::ThrowError("The first argument must be a string");
      return;
  }

  std::string path = (*Utf8String(info[0]));

  if (info.Length() == 1) {
    Nan::ThrowError("You must pass a second argument");
    return;
  }

  if (!info[1]->IsString()) {
    Nan::ThrowError("The second argument must be a string");
      return;
  }

  std::string type = (*Utf8String(info[1]));

  if (info.Length() == 2) {
    Nan::ThrowError("You must pass a third argument");
    return;
  }

  if (!info[2]->IsFunction()) {
    Nan::ThrowError("The third argument must be a callback function");
      return;
  }

  Callback *callback = new Callback(info[2].As<Function>());

  AsyncQueueWorker(new ParseWorker(callback, path, type));
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("parseFile").ToLocalChecked(),
           GetFunction(New<FunctionTemplate>(parseFile)).ToLocalChecked());
}

NODE_MODULE(addon, Init)
