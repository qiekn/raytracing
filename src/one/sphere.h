#pragma once

#include <cmath>
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
  Sphere(const point3& center, double radius)
      : center_(center), radius_(std::fmax(0, radius)) {}

  bool Hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override {
    https://tinyurl.com/5eynscbx
    vec3 oc = center_ - r.origin();
    double a = r.direction().length_squared();
    double h = dot(r.direction(), oc);
    double c = oc.length_squared() - radius_ * radius_;

    double discriminant = h * h - a * c;
    if (discriminant < 0)
      return false;

    double sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double t = (h - sqrtd) / a;
    if (t <= ray_tmin || t >= ray_tmax) {
      t = (h + sqrtd) / a;
      if (t <= ray_tmin || t >= ray_tmax)
        return false;
    }

    rec.t = t;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center_) / radius_;
    rec.SetFaceNormal(r, outward_normal);

    return true;
  }

private:
  point3 center_;
  double radius_;
};
