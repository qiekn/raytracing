#pragma once

#include "common.h"
#include "vec3.h"

// A simple Perlin-style noise class
class Perlin {
public:
  Perlin() {
    // Generate 256 random floating-point values in [0,1)
    for (int i = 0; i < point_count_; i++) {
      randfloat_[i] = RandomDouble();
    }

    // Generate permutation tables for x, y, and z directions
    PerlinGeneratePerm(perm_x_);
    PerlinGeneratePerm(perm_y_);
    PerlinGeneratePerm(perm_z_);
  }

  // Return a noise value for a given 3D point
  double Noise(const point3& p) const {
    // Scale the coordinates and convert to integer grid indices
    // The factor 4 controls the noise frequency
    auto i = int(4 * p.x()) & 255;
    auto j = int(4 * p.y()) & 255;
    auto k = int(4 * p.z()) & 255;

    // Hash the coordinates using permutation tables and XOR,
    // then use the result to index into the random value table
    return randfloat_[perm_x_[i] ^ perm_y_[j] ^ perm_z_[k]];
  }

private:
  static void PerlinGeneratePerm(int* p) {
    for (int i = 0; i < point_count_; i++)
      p[i] = i;

    Permute(p, point_count_);
  }

  // Fisher–Yates shuffle algorithm
  static void Permute(int* p, int n) {
    for (int i = n - 1; i > 0; i--) {
      // Pick a random index from [0, i]
      int target = RandomInt(0, i);

      // Swap elements
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

private:
  static const int point_count_ = 256;
  double randfloat_[point_count_];

  // perm -> permutation (置换表 / 随机排列表)
  int perm_x_[point_count_];
  int perm_y_[point_count_];
  int perm_z_[point_count_];
};
