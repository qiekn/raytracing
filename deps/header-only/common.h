#pragma once

#include <limits>
#include <memory>
#include <random>

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

// Returns a random real in [0, 1)
inline double RandomDouble() {
  // This is the old wary
  // return std::rand() / (RAND_MAX + 1.0);

  // Now we can...
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

// Returns a random real in [min, max)
inline double RandomDouble(double min, double max) {
  return min + (max - min) * RandomDouble();
}

// Returns a random integer in [min, max]
inline int RandomInt(int min, int max) {
  return int(RandomDouble(min, max + 1));
}
