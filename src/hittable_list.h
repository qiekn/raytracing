#pragma once

#include <memory>
#include <vector>
#include "aabb.h"
#include "hittable.h"
#include "interval.h"

class HittableList : public Hittable {
public:
  HittableList() {}

  HittableList(std::shared_ptr<Hittable> object) { Add(object); }

  void Clear() { objects_.clear(); }

  void Add(std::shared_ptr<Hittable> object) {
    objects_.push_back(object);
    bbox_ = AABB(bbox_, object->BoundingBox());
  }

  bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto& object : objects_) {
      if (object->Hit(r, Interval(ray_t.min, closest_so_far), temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hit_anything;
  }

  AABB BoundingBox() const override { return bbox_; }

public:
  std::vector<std::shared_ptr<Hittable>> objects_;

private:
  AABB bbox_;
};
