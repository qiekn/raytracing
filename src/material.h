#pragma once

#include <cmath>
#include "color.h"
#include "common.h"
#include "hittable.h"
#include "ray.h"
#include "texture.h"
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

  virtual color Emitted(double u, double v, const point3& p) const { return color(0, 0, 0); }
};

class Lambertian : public Material {
public:
  Lambertian(const color& albedo) : texture_(make_shared<SolidColor>(albedo)) {}

  Lambertian(shared_ptr<Texture> texture) : texture_(texture) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, color& attenuation,
               Ray& scattered) const override {
    vec3 scatter_direction = rec.normal + random_unit_vector();

    // Catch degenrate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    scattered = Ray(rec.p, scatter_direction, r_in.time());
    attenuation = texture_->Value(rec.u, rec.v, rec.p);
    return true;
  }

private:
  shared_ptr<Texture> texture_;
};

class Metal : public Material {
public:
  Metal(const color& albedo, double fuzz) : albedo_(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, color& attenuation,
               Ray& scattered) const override {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (fuzz_ * random_unit_vector());
    scattered = Ray(rec.p, reflected, r_in.time());
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

    if (cannot_refract || Reflectance(cos_theta, ri) > RandomDouble())
      direction = reflect(unit_direction, rec.normal);
    else
      direction = refract(unit_direction, rec.normal, ri);

    scattered = Ray(rec.p, direction, r_in.time());
    return true;
  }

private:
  static double Reflectance(double cosine, double refraction_index) {
    // Use Schlick's approximation forreflectance.
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
  }

private:
  // Refractive index in vacuum or air, or the ratio of the material's refractive
  // index over the refractive index of the enclasing media
  double refraction_index_;
};

class DiffuseLight : public Material {
public:
  DiffuseLight(shared_ptr<Texture> texture) : texture_(texture) {}

  DiffuseLight(const color& emit) : texture_(make_shared<SolidColor>(emit)) {}

  color Emitted(double u, double v, const point3& p) const override {
    return texture_->Value(u, v, p);
  }

private:
  shared_ptr<Texture> texture_;
};

class Isotropic : public Material {
public:
  Isotropic(const color& albedo) : texture_(make_shared<SolidColor>(albedo)) {}

  Isotropic(shared_ptr<Texture> texture) : texture_(texture) {}

  bool Scatter(const Ray& r_in, const HitRecord& rec, color& attenuation,
               Ray& scattered) const override {
    scattered = Ray(rec.p, random_unit_vector(), r_in.time());
    attenuation = texture_->Value(rec.u, rec.v, rec.p);
    return true;
  }

private:
  shared_ptr<Texture> texture_;
};
