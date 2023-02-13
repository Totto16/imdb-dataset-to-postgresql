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
      : AsyncWorker(callback), path(path){}

  ~ParseWorker() {}

  void Execute() {
    result = std::move(parse_data(path));
    // ERROROr<>
  }

  // We have the results, and we're back in the event loop.
  void HandleOKCallback() {
    Nan::HandleScope scope;

    v8::Local<v8::Array> results = New<v8::Array>(parsedValues.size());
    int i = 0;
    for_each(parsedValues.begin(), parsedValues.end(), [&](int value) {
      Nan::Set(results, i, New<v8::Number>(value));
      i++;
    });

    Local<Value> result[] = {Null(), results};

    callback->Call(2, result);
  }

private:
  std::string path;
  ParseResult result = nullptr;
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
