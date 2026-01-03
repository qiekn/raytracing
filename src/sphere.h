#pragma once

#include <cmath>
#include "aabb.h"
#include "common.h"
#include "hittable.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
  // Stationary Sphere
  Sphere(const point3& static_center, double radius, shared_ptr<Material> mat)
      : center_(static_center, vec3(0, 0, 0)), radius_(std::fmax(0, radius)), mat_(mat) {
    auto half_diag = vec3(radius_, radius_, radius_);
    bbox_ = AABB(static_center - half_diag, static_center + half_diag);
  }

  // Moving Sphere
  Sphere(const point3& center1, const point3& center2, double radius, shared_ptr<Material> mat)
      : center_(center1, center2 - center1), radius_(std::fmax(0, radius)), mat_(mat) {
    auto rvec = vec3(radius_, radius_, radius_);
    AABB box1(center_.at(0) - rvec, center_.at(0) + rvec);
    AABB box2(center_.at(1) - rvec, center_.at(1) + rvec);
    bbox_ = AABB(box1, box2);
  }

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
    GetSphereUV(outward_normal, rec.u, rec.v);
    rec.mat = mat_;

    return true;
  }

  AABB BoundingBox() const override { return bbox_; }

private:
  static void GetSphereUV(const point3& p, double& u, double& v) {
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + kPi;

    u = phi / (2 * kPi);
    v = theta / kPi;
  }

private:
  Ray center_;
  double radius_;
  shared_ptr<Material> mat_;
  AABB bbox_;
};
