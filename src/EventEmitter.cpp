#include <iostream>
#include <nan.h>
#include <sstream>
#include <thread>

#include "eventemitter.hpp"

using namespace std;
using namespace NodeEvent;
using namespace Nan;
using namespace v8;

class ReentrantWorker : public AsyncEventEmittingReentrantCWorker<16> {
public:
  ReentrantWorker(Nan::Callback *callback,
                  std::shared_ptr<EventEmitter> emitter, size_t n)
      : AsyncEventEmittingReentrantCWorker(callback, emitter), n_(n) {}

  virtual void ExecuteWithEmitter(const ExecutionProgressSender *sender,
                                  eventemitter_fn_r emitter) override {
    for (int32_t i = 0; i < n_; ++i) {
      stringstream ss;
      ss << "Test" << i;
      while (!emitter(sender, "test", ss.str().c_str())) {
        std::this_thread::yield();
      }
      while (!emitter(sender, "test2", ss.str().c_str())) {
        std::this_thread::yield();
      }
      while (!emitter(sender, "test3", ss.str().c_str())) {
        std::this_thread::yield();
      }
    }
  }

private:
  int32_t n_;
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

  static NAN_METHOD(Run) {
    Nan::Callback *fn(nullptr);
    if (info.Length() < 1 || info.Length() > 2) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments"));
      return;
    }
    if (!info[0]->IsNumber()) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("First argument must be number"));
      return;
    }
    if (info.Length() == 2) {
      if (info[1]->IsFunction()) {
        fn = new Nan::Callback(info[1].As<Function>());
      } else {
        info.GetIsolate()->ThrowException(
            Nan::TypeError("Second argument must be function"));
        return;
      }
    }

    int32_t n = info[0]->Int32Value(Nan::GetCurrentContext()).ToChecked();
    auto Parser = Nan::ObjectWrap::Unwrap<NativeParser>(info.Holder());

    ReentrantWorker *worker = new ReentrantWorker(fn, Parser->emitter_, n);
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
