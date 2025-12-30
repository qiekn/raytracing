#pragma once

#include "ray.h"
#include "vec3.h"

class HitRecord {
public:
  point3 p; // hit point
  double t; // ray parameter
  vec3 normal;
  bool front_face;

  void SetFaceNormal(const Ray& r, const vec3& outward_normal) {
    // Sets the hit record normal vector.
    // NOTE: the parameter `outward_normal` is assumed to have unit length.
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool Hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const = 0;
};
