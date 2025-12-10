

#include "./print.hpp"

#include <sstream>

// From: https://stackoverflow.com/questions/22063979/elegant-time-print-in-c11
static void prettyPrintImpl(std::ostream &out,
                            const std::chrono::milliseconds &input) {
  auto diff = input;
  auto const msecs = diff % 1000;
  diff /= 1000;
  auto const secs = diff % 60;
  diff /= 60;
  auto const mins = diff % 60;
  diff /= 60;
  auto const hours = diff % 24;
  diff /= 24;
  auto const days = diff;

  bool printed_earlier = false;
  if (days.count() >= 1) {
    printed_earlier = true;
    out << days.count() << (1 != days.count() ? " days" : " day") << ' ';
  }
  if (printed_earlier || hours.count() >= 1) {
    printed_earlier = true;
    out << hours.count() << (1 != hours.count() ? " hours" : " hour") << ' ';
  }
  if (printed_earlier || mins.count() >= 1) {
    printed_earlier = true;
    out << mins.count() << (1 != mins.count() ? " minutes" : " minute") << ' ';
  }
  if (printed_earlier || secs.count() >= 1) {
    printed_earlier = true;
    out << secs.count() << (1 != secs.count() ? " seconds" : " second") << ' ';
  }
  if (printed_earlier || msecs.count() >= 1) {
    out << msecs.count()
        << (1 != msecs.count() ? " milliseconds" : " millisecond");
  }
}

[[nodiscard]] std::string prettyPrint(const std::chrono::milliseconds &diff) {
  std::stringstream str{};
  prettyPrintImpl(str, diff);
  return str.str();
}
