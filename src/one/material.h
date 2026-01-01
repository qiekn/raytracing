#pragma once

#include <cmath>
#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

// From: The color is determined by the light hitting the geometry
// Upgraded to: The material decides how to reflect/scatter the light
class Material {
public:
  virtual ~Material() = default;

  virtual bool Scatter(const Ray& r_in, const HitRecord& rec, color& attenuation,
                       Ray& scattered) const {
    return false;
  }
};

class Lambertian : public Material {
public:
  Lambertian(const color& albedo) : albedo_(albedo) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, color& attenuation,
               Ray& scattered) const override {
    vec3 scatter_direction = rec.normal + random_unit_vector();

    // Catch degenrate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    scattered = Ray(rec.p, scatter_direction);
    attenuation = albedo_;
    return true;
  }

private:
  color albedo_;
};

class Metal : public Material {
public:
  Metal(const color& albedo, double fuzz) : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, color& attenuation,
               Ray& scattered) const override {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (fuzz_ * random_unit_vector());
    scattered = Ray(rec.p, reflected);
    attenuation = albedo_;
    return (dot(scattered.direction(), rec.normal) > 0);
  }

private:
  color albedo_;
  double fuzz_;
};

class Dielectric : public Material {
public:
  Dielectric(double refraction_index) : refraction_index_(refraction_index) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, color& attenuation,
               Ray& scattered) const override {
    attenuation = color(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0 / refraction_index_) : refraction_index_;

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract)
      direction = reflect(unit_direction, rec.normal);
    else
      direction = refract(unit_direction, rec.normal, ri);

    scattered = Ray(rec.p, direction);
    return true;
  }

private:
  // Refractive index in vacuum or air, or the ratio of the material's refractive
  // index over the refractive index of the enclasing media
  double refraction_index_;
};
