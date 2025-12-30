#pragma once

#include "common.h"

class Interval {
public:
  Interval() : min(kInfinity), max(-kInfinity) {}

  Interval(double min, double max) : min(min), max(max) {}

  double Size() const { return max - min; }

  bool Contains(double x) const { return x >= min && x <= max; }

  bool Surrounds(double x) const { return x > min && x < max; }

  double Clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
  }

  static const Interval empty, universe;

public:
  double min, max;
};

const Interval Interval::empty    = Interval( kInfinity, -kInfinity);
const Interval Interval::universe = Interval(-kInfinity,  kInfinity);
