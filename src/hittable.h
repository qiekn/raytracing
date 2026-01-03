#pragma once

#include <cmath>
#include "aabb.h"
#include "common.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

class Material;

class HitRecord {
public:
  point3 p;  // hit point
  double t;  // ray parameter
  double u;
  double v;
  vec3 normal;
  bool front_face;
  shared_ptr<Material> mat;

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

  virtual bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;

  virtual AABB BoundingBox() const = 0;
};

class Translate : public Hittable {
public:
  Translate(shared_ptr<Hittable> object, const vec3& offset)
      : object_(object), offset_(offset) {
    bbox_ = object_->BoundingBox() + offset_;
  }

  bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
    // Move the ray backwards by the offset
    Ray offset_r(r.origin() - offset_, r.direction(), r.time());

    // Determine whether an intersection exist along the offset ray (and if so, where)
    if (!object_->Hit(offset_r, ray_t, rec))
      return false;

    // Move the intersection point forward by the offset
    rec.p += offset_;
    return true;
  }

  AABB BoundingBox() const override { return bbox_; }

private:
  shared_ptr<Hittable> object_;
  vec3 offset_;
  AABB bbox_;
};

class RotateY : public Hittable {
public:
  RotateY(shared_ptr<Hittable> object, double angle) : object_(object) {
    auto radians = Deg2Rad(angle);
    sin_theta_ = std::sin(radians);
    cos_theta_ = std::cos(radians);
    bbox_ = object->BoundingBox();

    point3 min(kInfinity, kInfinity, kInfinity);
    point3 max(-kInfinity, -kInfinity, -kInfinity);

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          auto x = i * bbox_.x.max + (1 - i) * bbox_.x.min;
          auto y = j * bbox_.y.max + (1 - j) * bbox_.y.min;
          auto z = k * bbox_.z.max + (1 - k) * bbox_.z.min;

          auto new_x = cos_theta_ * x + sin_theta_ * z;
          auto new_z = -sin_theta_ * x + cos_theta_ * z;

          vec3 tester(new_x, y, new_z);

          for (int c = 0; c < 3; c++) {
            min[c] = std::fmin(min[c], tester[c]);
            max[c] = std::fmax(max[c], tester[c]);
          }
        }
      }
    }

    bbox_ = AABB(min, max);
  }

  bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
    // Transform the ray from world space to object space
    // clang-format off
    auto origin = point3(
      (cos_theta_ * r.origin().x()) - (sin_theta_ * r.origin().z()),
      r.origin().y(),
      (sin_theta_ * r.origin().x()) + (cos_theta_ * r.origin().z())
    );
    auto direction = vec3(
      (cos_theta_ * r.direction().x()) - (sin_theta_ * r.direction().z()),
      r.direction().y(),
      (sin_theta_ * r.direction().x()) + (cos_theta_ * r.direction().z())
    );

    Ray rotate_r(origin, direction, r.time());

    // Determin whether an intersection exists in object space (and if so, where)
    if (!object_->Hit(rotate_r, ray_t, rec))
      return false;

    // Transform the intersection from object space back to world space.
    rec.p = point3(
      (cos_theta_ * rec.p.x()) + (sin_theta_ * rec.p.z()),
      rec.p.y(),
      (-sin_theta_ * rec.p.x()) + (cos_theta_ * rec.p.z())
    );

    rec.normal = vec3(
      (cos_theta_ * rec.normal.x()) + (sin_theta_ * rec.normal.z()),
      rec.normal.y(),
      (-sin_theta_ * rec.normal.x()) + (cos_theta_ * rec.normal.z())
    );

    // clang-format on
    return true;
  }

  AABB BoundingBox() const override { return bbox_; }

private:
  shared_ptr<Hittable> object_;
  double sin_theta_;
  double cos_theta_;
  AABB bbox_;
};
