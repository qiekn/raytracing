#pragma once

#include "common.h"

class Interval {
public:
  Interval() : min(kInfinity), max(-kInfinity) {}

  Interval(double min, double max) : min(min), max(max) {}

  // Create the interval tightly enclosing the tow input intervals
  Interval(const Interval& a,const Interval& b) {
    min = std::min(a.min, b.min);
    max = std::max(a.max, b.max);
  }

  double Size() const { return max - min; }

  bool Contains(double x) const { return x >= min && x <= max; }

  bool Surrounds(double x) const { return x > min && x < max; }

  double Clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
  }

  Interval Expand(double delta) const {
    auto padding = delta / 2;
    return Interval(min - padding, max + padding);

  }

  static const Interval empty, universe;

public:
  double min, max;
};

const Interval Interval::empty    = Interval( kInfinity, -kInfinity);
const Interval Interval::universe = Interval(-kInfinity,  kInfinity);

Interval operator+(const Interval& ival, double displacement) {
  return Interval(ival.min + displacement, ival.max + displacement);
}

Interval operator+(double displacement, const Interval& ival) {
  return ival + displacement;
}
