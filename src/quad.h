#pragma once

#include <cmath>
#include <functional>
#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "material.h"
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

  virtual bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
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

protected:
  point3 origin_;
  vec3 u_, v_;
  vec3 w_;
  shared_ptr<Material> mat_;
  AABB bbox_;

  vec3 normal_;
  double D_;
};

// ----------------------------------------------------------------------------: easter eggs
class Triangle : public Quad {
public:
  Triangle(const point3& o, const vec3& aa, const vec3& ab, shared_ptr<Material> mat)
      : Quad(o, aa, ab, mat) {}

  virtual bool IsInterior(double a, double b, HitRecord& rec) const override {
    if ((a < 0) || (b < 0) || (a + b > 1))
      return false;

    rec.u = a;
    rec.v = b;
    return true;
  }
};

class Ellipse : public Quad {
public:
  Ellipse(const point3& center, const vec3& u, const vec3& v, shared_ptr<Material> mat)
      : Quad(center, u, v, mat) {}

  void SetBoundingBox() override { bbox_ = AABB(origin_ - u_ - v_, origin_ + u_ + v_); }

  bool IsInterior(double a, double b, HitRecord& rec) const override {
    if ((a * a + b * b) > 1)
      return false;

    rec.u = a / 2 + 0.5;
    rec.v = b / 2 + 0.5;
    return true;
  }
};

// 圆环
class Annulus : public Quad {
public:
  Annulus(const point3& center, const vec3& u, const vec3& v, double inner,
          shared_ptr<Material> mat)
      : Quad(center, u, v, mat), inner_(inner) {}

  void SetBoundingBox() override { bbox_ = AABB(origin_ - u_ - v_, origin_ + u_ + v_); }

  bool IsInterior(double a, double b, HitRecord& rec) const override {
    auto center_dist = std::sqrt(a * a + b * b);
    if ((center_dist < inner_) || (center_dist > 1))
      return false;

    rec.u = a / 2 + 0.5;
    rec.v = b / 2 + 0.5;
    return true;
  }

private:
  double inner_;
};

// ----------------------------------------------------------------------------: box
// Return the 3D box (six side) that contains the tow opposite vertices a & b
inline shared_ptr<HittableList> box(const point3& a, const point3& b, shared_ptr<Material> mat) {
  auto sides = make_shared<HittableList>();

  // Construct the two opposite vertices with the minimum and maximum coordinates
  auto min = point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
  auto max = point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

  auto dx = vec3(max.x() - min.x(), 0, 0);
  auto dy = vec3(0, max.y() - min.y(), 0);
  auto dz = vec3(0, 0, max.z() - min.z());

  sides->Add(make_shared<Quad>(point3(min.x(), min.y(), max.z()), dx, dy, mat));   // front
  sides->Add(make_shared<Quad>(point3(max.x(), min.y(), max.z()), -dz, dy, mat));  // right
  sides->Add(make_shared<Quad>(point3(max.x(), min.y(), min.z()), -dx, dy, mat));  // back
  sides->Add(make_shared<Quad>(point3(min.x(), min.y(), min.z()), dz, dy, mat));   // left
  sides->Add(make_shared<Quad>(point3(min.x(), max.y(), max.z()), dx, -dz, mat));  // top
  sides->Add(make_shared<Quad>(point3(min.x(), min.y(), min.z()), dx, dz, mat));   // bottom

  return sides;
}
