#pragma once

#include "color.h"
#include "common.h"

class Texture {
public:
  virtual ~Texture() = default;

  virtual color Value(double u, double v, const point3& p) const = 0;
};

class SolidColor : public Texture {
public:
  SolidColor(const color& albedo) : albedo_(albedo) {}

  SolidColor(double r, double g, double b) : SolidColor(color(r, g, b)) {}

  color Value(double u, double v, const point3& p) const override { return albedo_; }

private:
  color albedo_;
};

class CheckerTexture : public Texture {
public:
  CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd)
      : scale_(1.0 / scale), even_(even), odd_(odd) {}

  CheckerTexture(double scale, const color& c1, const color& c2)
      : CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

  color Value(double u, double v, const point3& p) const override {
    int x = int(std::floor(scale_ * p.x()));
    int y = int(std::floor(scale_ * p.y()));
    int z = int(std::floor(scale_ * p.z()));

    bool is_even = (x + y + z) % 2 == 0;

    return is_even ? even_->Value(u, v, p) : odd_->Value(u, v, p);
  }

private:
  double scale_;
  shared_ptr<Texture> even_;
  shared_ptr<Texture> odd_;
};
