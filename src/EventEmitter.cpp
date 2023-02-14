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
      while (!emitter((void *)sender, "test", ss.str().c_str())) {
        std::this_thread::yield();
      }
      while (!emitter((void *)sender, "test2", ss.str().c_str())) {
        std::this_thread::yield();
      }
      while (!emitter((void *)sender, "test3", ss.str().c_str())) {
        std::this_thread::yield();
      }
    }
  }

private:
  int32_t n_;
};

class EmittingThing : public Nan::ObjectWrap {
public:
  static NAN_MODULE_INIT(Init) {
    auto clsName = Nan::New("EmitterThing").ToLocalChecked();
    auto constructor = Nan::New<v8::FunctionTemplate>(New);
    auto tpl = constructor->InstanceTemplate();
    constructor->SetClassName(clsName);
    tpl->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(constructor, "on", On);
    Nan::SetPrototypeMethod(constructor, "runReentrant", RunReentrant);
    Nan::SetPrototypeMethod(constructor, "removeAllListeners",
                            RemoveAllListeners);
    Nan::SetPrototypeMethod(constructor, "eventNames", EventNames);

    Nan::Set(target, clsName, Nan::GetFunction(constructor).ToLocalChecked());
  };

private:
  EmittingThing() : emitter_(std::make_shared<EventEmitter>()) {}
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

    auto thing = Nan::ObjectWrap::Unwrap<EmittingThing>(info.Holder());
    thing->emitter_->on(s, callback);
  }

  static NAN_METHOD(RemoveAllListeners) {
    if (info.Length() > 1) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments"));
      return;
    }
    auto thing = Nan::ObjectWrap::Unwrap<EmittingThing>(info.Holder());

    if (info.Length() == 1) {
      if (!info[0]->IsString()) {
        info.GetIsolate()->ThrowException(
            Nan::TypeError("First argument must be string name of an event"));
        return;
      }
      auto ev = *Nan::Utf8String(info[0]);
      thing->emitter_->removeAllListenersForEvent(ev);
    } else {
      thing->emitter_->removeAllListeners();
    }
  }

  static NAN_METHOD(EventNames) {
    if (info.Length() > 0) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("Wrong number of arguments"));
      return;
    }
    auto thing = Nan::ObjectWrap::Unwrap<EmittingThing>(info.Holder());
    v8::Local<v8::Array> v = v8::Array::New(info.GetIsolate());

    size_t i = 0;
    for (auto s : thing->emitter_->eventNames()) {
      Nan::Set(v, i, (Nan::New<v8::String>(s)).ToLocalChecked());
      ++i;
    }

    info.GetReturnValue().Set(v);
  }

  static NAN_METHOD(RunReentrant) {
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
    auto thing = Nan::ObjectWrap::Unwrap<EmittingThing>(info.Holder());

    ReentrantWorker *worker = new ReentrantWorker(fn, thing->emitter_, n);
    Nan::AsyncQueueWorker(worker);
  }

  static NAN_METHOD(New) {
    if (!info.IsConstructCall()) {
      info.GetIsolate()->ThrowException(
          Nan::TypeError("call to constructor without keyword new"));
      return;
    }

    EmittingThing *o = new EmittingThing();
    o->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  std::shared_ptr<NodeEvent::EventEmitter> emitter_;
};

NAN_MODULE_INIT(InitAll) { EmittingThing::Init(target); }
NODE_MODULE(NanObject, InitAll);
