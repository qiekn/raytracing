#pragma once

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
  Translate(shared_ptr<Hittable> object, const vec3& offset) : object_(object), offset_(offset) {
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
