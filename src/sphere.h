#pragma once

#include <cmath>
#include "common.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
  // Stationary Sphere
  Sphere(const point3& static_center, double radius, shared_ptr<Material> mat)
      : center_(static_center, vec3(0, 0, 0)), radius_(std::fmax(0, radius)), mat_(mat) {}

  // Moving Sphere
  Sphere(const point3& center1, const point3& center2, double radius, shared_ptr<Material> mat)
      : center_(center1, center2 - center1), radius_(std::fmax(0, radius)), mat_(mat) {}

  bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
    https://tinyurl.com/5eynscbx
    point3 current_center = center_.at(r.time());
    vec3 oc = current_center - r.origin();
    double a = r.direction().length_squared();
    double h = dot(r.direction(), oc);
    double c = oc.length_squared() - radius_ * radius_;

    double discriminant = h * h - a * c;
    if (discriminant < 0)
      return false;

    double sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double t = (h - sqrtd) / a;
    if (!ray_t.Surrounds(t)) {
      t = (h + sqrtd) / a;
      if (!ray_t.Surrounds(t))
        return false;
    }

    rec.t = t;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - current_center) / radius_;
    rec.SetFaceNormal(r, outward_normal);
    rec.mat = mat_;

    return true;
  }

private:
  Ray center_;
  double radius_;
  shared_ptr<Material> mat_;
};
