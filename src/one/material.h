#pragma once

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
  Metal(const color& albedo) : albedo_(albedo) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, color& attenuation,
               Ray& scattered) const override {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    scattered = Ray(rec.p, reflected);
    attenuation = albedo_;
    return true;
  }

private:
  color albedo_;
};
