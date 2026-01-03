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
    auto u = p.x() - std::floor(p.x());
    auto v = p.y() - std::floor(p.y());
    auto w = p.z() - std::floor(p.z());

    auto i = int(std::floor(p.x()));
    auto j = int(std::floor(p.y()));
    auto k = int(std::floor(p.z()));
    double c[2][2][2];

    for (int di = 0; di < 2; di++)
      for (int dj = 0; dj < 2; dj++)
        for (int dk = 0; dk < 2; dk++)
          // clang-format off
          c[di][dj][dk] = randfloat_[
            perm_x_[(i + di) & 255] ^
            perm_y_[(j + dj) & 255] ^
            perm_z_[(k + dk) & 255]
          ];
          // clang-format on

    return TrilinearInterp(c, u, v, w);
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

  static double TrilinearInterp(double c[2][2][2], double u, double v, double w) {
    auto accum = 0.0;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        for (int k = 0; k < 2; k++)
          // clang-format off
          accum += (i * u + (1 - i) * (1 - u)) *
                   (j * v + (1 - j) * (1 - v)) *
                   (k * w + (1 - k) * (1 - w)) *
                   c[i][j][k];
          // clang-format on

    return accum;
  }

private:
  static const int point_count_ = 256;
  double randfloat_[point_count_];

  // perm -> permutation (置换表 / 随机排列表)
  int perm_x_[point_count_];
  int perm_y_[point_count_];
  int perm_z_[point_count_];
};
