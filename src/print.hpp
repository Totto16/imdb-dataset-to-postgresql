#pragma once

#include <chrono>
#include <string>

[[nodiscard]] std::string prettyPrint(const std::chrono::milliseconds &diff);
