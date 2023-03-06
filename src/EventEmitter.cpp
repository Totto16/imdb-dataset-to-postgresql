#include <iostream>
#include <memory>
#include <nan.h>
#include <sstream>
#include <thread>

#include "TSVParser.hpp"
#include "helper.hpp"

using namespace std;
using namespace NodeEvent;
using namespace Nan;
using namespace v8;

// DOCS:

// https://github.com/Totto16/imdb-dataset
// https://github.com/Totto16/imdb-dataset-parser
// https://github.com/nodejs/nan/blob/main/doc/asyncworker.md#api_nan_async_progress_worker
// https://github.com/dagronf/csvlib
// https://github.com/Totto16/cpp-eventemitter
// https://v8docs.nodesource.com/node-18.2/de/deb/classv8_1_1_local.html
// https://gyp.gsrc.io/docs/UserDocumentation.md#Add-a-new-library

// use a RingBuffer with 64 capacity (was 16 initially)!
class ReentrantWorker
    : public AsyncEventEmittingReentrantCWorker<RingBufferSize> {
public:
  ReentrantWorker(Nan::Callback *callback,
                  std::shared_ptr<EventEmitter> emitter, string filePath,
                  string type, OmitHeadType hasHead)
      : AsyncEventEmittingReentrantCWorker(callback, emitter),
        filePath{move(filePath)}, type{move(type)}, hasHead{hasHead} {}

  virtual void
  ExecuteWithEmitter(const ExecutionProgressSender *sender,
                     EventEmitterFunctionReentrant emitter) override {
    MaybeParser maybeParser = makeParser(filePath, type);

    if (!maybeParser.has_value()) {
      ParseResult result = tl::make_unexpected(maybeParser.error());

      std::string error = result.error();

      std::shared_ptr<Constructable> typeErrorVal =
          std::make_shared<TypeErrorConstructable>(error);
      while (!emitter(sender, "error", typeErrorVal)) {
        std::this_thread::yield();
      }
      return;
    } else {
      auto parser = maybeParser.value();
      ParseResult result = parser.parseData(sender, emitter);

      if (!result.has_value()) {

        std::string error = result.error();
        std::shared_ptr<Constructable> typeErrorVal =
            std::make_shared<TypeErrorConstructable>(error);
        while (!emitter(sender, "error", typeErrorVal)) {
          std::this_thread::yield();
        }
        return;
      }

      auto value = result.value();
      if (value) {
        std::shared_ptr<Constructable> endVal =
            std::make_shared<UndefinedConstructable>();
        while (!emitter(sender, "end", endVal)) {
          std::this_thread::yield();
        }
      } else {
        std::shared_ptr<Constructable> unreachableVal =
            std::make_shared<TypeErrorConstructable>("UNREACHABLE");
        while (!emitter(sender, "error", unreachableVal)) {
          std::this_thread::yield();
        }
        return;
      }
    }
  }

private:
  string filePath;
  string type;
  OmitHeadType hasHead;
};

class NativeParser : public Nan::ObjectWrap {
public:
  static NAN_MODULE_INIT(Init) {
    auto clsName = Nan::New("NativeParser").ToLocalChecked();
    auto constructor = Nan::New<v8::FunctionTemplate>(New);
    auto tpl = constructor->InstanceTemplate();
    constructor->SetClassName(clsName);
    tpl->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(constructor, "on", On);
    Nan::SetPrototypeMethod(constructor, "run", Run);
    Nan::SetPrototypeMethod(constructor, "resume", Resume);
    Nan::SetPrototypeMethod(constructor, "pause", Pause);
    Nan::SetPrototypeMethod(constructor, "removeAllListeners",
                            RemoveAllListeners);
    Nan::SetPrototypeMethod(constructor, "eventNames", EventNames);

    Nan::Set(target, clsName, Nan::GetFunction(constructor).ToLocalChecked());
  };

private:
  NativeParser() : emitter_(std::make_shared<EventEmitter>()) {}
  static NAN_METHOD(On) {
    if (info.Length() != 2) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments"));
      return;
    }
    if (!info[0]->IsString()) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("First argument must be string"));
      return;
    }
    if (!info[1]->IsFunction()) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Second argument must be function"));
      return;
    }

    auto s = *Nan::Utf8String(info[0]);
    Nan::Callback *callback = new Nan::Callback(info[1].As<Function>());

    auto Parser = Nan::ObjectWrap::Unwrap<NativeParser>(info.Holder());
    Parser->emitter_->on(s, callback);
  }

  static NAN_METHOD(RemoveAllListeners) {
    if (info.Length() > 1) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments"));
      return;
    }
    auto Parser = Nan::ObjectWrap::Unwrap<NativeParser>(info.Holder());

    if (info.Length() == 1) {
      if (!info[0]->IsString()) {
        info.GetIsolate()->ThrowException(
            Nan::TypeError("First argument must be string name of an event"));
        return;
      }
      auto ev = *Nan::Utf8String(info[0]);
      Parser->emitter_->removeAllListenersForEvent(ev);
    } else {
      Parser->emitter_->removeAllListeners();
    }

    info.GetReturnValue().Set(info.This());
  }

  static NAN_METHOD(EventNames) {
    if (info.Length() > 0) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments"));
      return;
    }
    auto Parser = Nan::ObjectWrap::Unwrap<NativeParser>(info.Holder());
    v8::Local<v8::Array> v = v8::Array::New(info.GetIsolate());

    size_t i = 0;
    for (auto s : Parser->emitter_->eventNames()) {
      Nan::Set(v, i, (Nan::New<v8::String>(s)).ToLocalChecked());
      ++i;
    }

    info.GetReturnValue().Set(v);
  }

  // TODO implement correct
  static NAN_METHOD(Resume) {
    if (info.Length() > 0) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments"));
      return;
    }
    // auto Parser = Nan::ObjectWrap::Unwrap<NativeParser>(info.Holder());

    info.GetReturnValue().Set(Nan::Null());
  }

  // TODO implement correct
  static NAN_METHOD(Pause) { // TODO
    if (info.Length() > 0) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments"));
      return;
    }
    //  auto Parser = Nan::ObjectWrap::Unwrap<NativeParser>(info.Holder());

    info.GetReturnValue().Set(Nan::Null());
  }

  static NAN_METHOD(Run) {
    // ts: run(filePath: string, type: string, hasHead?: "auto" | boolean): void
    if (info.Length() < 2 || info.Length() > 3) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments: expected 2 or 3"));
      return;
    }
    if (!info[0]->IsString()) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("The first argument must be a string"));
      return;
    }

    if (!info[1]->IsString()) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("The second argument must be a string"));
      return;
    }

    string filePath = (*Utf8String(info[0]));
    string type = (*Utf8String(info[1]));

    OmitHeadType hasHead = OmitHeadType::Auto;

    if (info.Length() == 3) {
      if (info[2]->IsString()) {
        string hasHeadString = (*Utf8String(info[2]));
        if (hasHeadString == "auto") {
          hasHead = OmitHeadType::Auto;
        } else {
          info.GetIsolate()->ThrowException(
              Nan::TypeError("The third argument must be the string 'auto', "
                             "not another string"));
          return;
        }
      } else if (info[2]->IsBoolean()) {
        bool hasHeadBool = info[2]->BooleanValue(info.GetIsolate());
        if (hasHeadBool) {
          hasHead = OmitHeadType::True;
        } else {
          hasHead = OmitHeadType::False;
        }
      } else {
        info.GetIsolate()->ThrowException(
            Nan::TypeError("The third argument must be boolean or 'auto'"));
        return;
      }
    }

    auto Parser = Nan::ObjectWrap::Unwrap<NativeParser>(info.Holder());

    // TODO: either remove this callback or use it for some purpose
    Nan::Callback *fn(nullptr);

    ReentrantWorker *worker =
        new ReentrantWorker(fn, Parser->emitter_, filePath, type, hasHead);
    Nan::AsyncQueueWorker(worker);
  }

  static NAN_METHOD(New) {
    if (!info.IsConstructCall()) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("call to constructor without keyword new"));
      return;
    }

    NativeParser *o = new NativeParser();
    o->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  std::shared_ptr<NodeEvent::EventEmitter> emitter_;
};

NAN_MODULE_INIT(InitAll) { NativeParser::Init(target); }
NODE_MODULE(NanObject, InitAll);