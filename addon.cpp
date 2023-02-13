#include <functional>
#include <iostream>
#include <nan.h>

#include "src/TSVParser.hpp"

// inspired by https://github.com/freezer333/cppwebify-tutorial

using namespace Nan;
using namespace std;
using namespace v8;

// template <typename T>

class ParseWorker : public AsyncWorker {
public:
  ParseWorker(Callback *callback, std::string path)
      : AsyncWorker(callback), path(path) {}

  ~ParseWorker() {}

  void Execute() { result = std::move(parse_data(path)); }

  // We have the results, and we're back in the event loop.
  void HandleOKCallback() {
    Nan::HandleScope scope;

    if (result.has_value()) {

      auto resultVector = result.value();

      v8::Local<v8::Array> results = New<v8::Array>(resultVector.size());
      int i = 0;
      for_each(resultVector.begin(), resultVector.end(), [&](int value) {
        Nan::Set(results, i, New<v8::Number>(value));
        i++;
      });

      Local<Value> okResult[] = {Null(), results};

      callback->Call(2, okResult, async_resource);
    } else {

      auto value = result.error();

      Local<Value> errorResult[] = {Nan::Error(value.c_str()), Null()};

      callback->Call(2, errorResult, async_resource);
    }
  }

private:
  std::string path;
  ParseResult result;
};

// Asynchronous access to the `getPrimes()` function
NAN_METHOD(parseFile) {

  if (!info[0]->IsString()) {
    Nan::ThrowError("The first argument must be a string!");
  }

  std::string path = (*Utf8String(info[0]));

  if (!info[1]->IsFunction()) {
    Nan::ThrowError("The second argument must be a callback function!");
  }

  Callback *callback = new Callback(info[1].As<Function>());

  AsyncQueueWorker(new ParseWorker(callback, path));
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, New<String>("parseFile").ToLocalChecked(),
           GetFunction(New<FunctionTemplate>(parseFile)).ToLocalChecked());
}

NODE_MODULE(addon, Init)
