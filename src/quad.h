#pragma once

#include "common.h"
#include "hittable.h"
#include "interval.h"
#include "vec3.h"

class Quad : public Hittable {
public:
  Quad(const point3& origin, const vec3& u, const vec3& v, shared_ptr<Material> mat)
      : origin_(origin), u_(u), v_(v), mat_(mat) {
    vec3 n = cross(u_, v_);
    normal_ = unit_vector(n);
    D_ = dot(normal_, origin_);
    w_ = n / dot(n, n);
  }

  // Compute the bounding box of all four vertices
  virtual void SetBoundingBox() {
    auto bbox_diagonal1 = AABB(origin_, origin_ + u_ + v_);
    auto bbox_diagonal2 = AABB(origin_ + u_, origin_ + v_);

    bbox_ = AABB(bbox_diagonal1, bbox_diagonal2);
  }

  AABB BoundingBox() const override { return bbox_; }

  bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
    auto denom = dot(normal_, r.direction());  // denom 是分母项的意思

    // No hit if the ray is parallel to the plane.
    if (std::fabs(denom) < 1e-8)
      return false;

    // Return false if the hit point parameter t is outside the ray interval.
    auto t = (D_ - dot(normal_, r.origin())) / denom;
    if (!ray_t.Contains(t))
      return false;

    // Determine if the hit point lies within the planar shape using its plane coordinates.
    auto intersection = r.at(t);
    vec3 planar_hitpt_vector = intersection - origin_;
    auto alpha = dot(w_, cross(planar_hitpt_vector, v_));
    auto beta = dot(w_, cross(u_, planar_hitpt_vector));

    if (!IsInterior(alpha, beta, rec))
      return false;

    // Ray hits the 2D shape; set the rest of the hit record and return true.
    rec.t = t;
    rec.p = intersection;
    rec.mat = mat_;
    rec.SetFaceNormal(r, normal_);

    return true;
  }

  virtual bool IsInterior(double a, double b, HitRecord& rec) const {
    Interval unit_interval = Interval(0, 1);
    // Given the hit point in plane coordinates, return false if it is outside the
    // primitive, otherwise set the hit record UV coordinates and return true.

    if (!unit_interval.Contains(a) || !unit_interval.Contains(b))
      return false;

    rec.u = a;
    rec.v = b;
    return true;
  }

private:
  point3 origin_;
  vec3 u_, v_;
  vec3 w_;
  shared_ptr<Material> mat_;
  AABB bbox_;

  vec3 normal_;
  double D_;
};
