#pragma once

#include <memory>
#include <vector>
#include "hittable.h"

class HittableList : Hittable {
public:
  HittableList() {}

  HittableList(std::shared_ptr<Hittable> object) {}

  void Clear() { objects_.clear(); }

  void Add(std::shared_ptr<Hittable> object) { objects_.push_back(object); }

  bool Hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_tmax;

    for (const auto& object : objects_) {
      if (object->Hit(r, ray_tmin, closest_so_far, temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hit_anything;
  }

public:
  std::vector<std::shared_ptr<Hittable>> objects_;
};
