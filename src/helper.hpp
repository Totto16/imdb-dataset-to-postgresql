

#pragma once

#include "async_queued_progress_worker.hpp"
#include "eventemitter.hpp"
#include <nan.h>

constexpr int RingBufferSize = 64;

using namespace Nan;
using namespace NodeEvent;

using ExecutionProgressSender =
    AsyncQueuedProgressWorker<EventEmitter::ProgressReport,
                              RingBufferSize>::ExecutionProgressSender;

using EventEmitterFunctionReentrant =
    std::function<int(const ExecutionProgressSender *sender,
                      const std::string event, const EventValue value)>;