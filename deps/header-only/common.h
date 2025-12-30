#pragma once

#include <limits>
#include <memory>

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double kInfinity = std::numeric_limits<double>::infinity();
const double kPi = 3.1415926535897932385;

// Utility Functions
inline double Deg2Rad(double degress) {
  return degress * kPi / 180.0;
}
