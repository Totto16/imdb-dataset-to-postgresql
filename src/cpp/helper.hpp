

#pragma once
#ifndef _NODE_EVENT_EMITTING_HELPER_H_
#define _NODE_EVENT_EMITTING_HELPER_H_

#include <nan.h>

#include "eventemitter.hpp"

constexpr int RingBufferSize = 16;

using namespace Nan;
using namespace NodeEvent;

using ExecutionProgressSender =
    AsyncQueuedProgressWorker<EventEmitter::ProgressReport,
                              RingBufferSize>::ExecutionProgressSender;

using EventEmitterFunctionReentrant =
    std::function<int(const ExecutionProgressSender *sender,
                      const std::string event, const EventValue value)>;

#endif
