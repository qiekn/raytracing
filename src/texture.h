#pragma once

#include "color.h"
#include "common.h"
#include "perlin.h"
#include "rtw_image.h"

// ----------------------------------------------------------------------------: base class
class Texture {
public:
  virtual ~Texture() = default;

  virtual color Value(double u, double v, const point3& p) const = 0;
};

// ----------------------------------------------------------------------------: derived class
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

class ImageTexture : public Texture {
public:
  ImageTexture(const char* filepath) : image_(filepath) {}

  color Value(double u, double v, const point3& p) const override {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (image_.height() <= 0)
      return color(0, 1, 1);

    // Clamp input texture corrdinates to [0, 1] x [1, 0]
    u = Interval(0, 1).Clamp(u);
    v = 1.0 - Interval(0, 1).Clamp(v);  // Flip V to image coordinates

    int i = int(u * image_.width());
    int j = int(v * image_.height());
    auto pixel = image_.pixel_data(i, j);

    auto color_scale = 1.0 / 255.0;
    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
  }

private:
  rtw_image image_;
};

class NoiseTexture : public Texture {
public:
  NoiseTexture(double scale) : scale_(scale) {}

  color Value(double u, double v, const point3& p) const override {
    return color(1,1,1) * perlin_.Noise(scale_ * p);
  }
  
private:
  Perlin perlin_;
  double scale_;
};
