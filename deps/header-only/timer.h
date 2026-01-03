#pragma once

#include <chrono>

class Timer {
  using Clock = std::chrono::steady_clock;

 public:
  Timer() : start_(Clock::now()) {}

  void Reset() { start_ = Clock::now(); }

  // Returns elapsed time in seconds
  double Elapsed() const {
    return std::chrono::duration<double>(Clock::now() - start_).count();
  }

 private:
  Clock::time_point start_;
};
