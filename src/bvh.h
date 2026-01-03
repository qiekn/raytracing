#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>
#include "aabb.h"
#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "ray.h"

class BvhNode : public Hittable {
public:
  BvhNode(HittableList list) : BvhNode(list.objects_, 0, list.objects_.size()) {
    // There's a C++ subtlety here. This constructor (without span indices) creates an
    // implicit copy of the hittable list, which we will modify. The lifetime of the copied
    // list only extends until this constructor exits. That's OK, because we only need to
    // persist the resulting bounding volume hierarchy.
  }

  BvhNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end) {
    // Randomly choose an axis
    int axis = RandomInt(0, 2);
    // clang-format off
    auto comparator = (axis == 0) ? BoxCompareX
                    : (axis == 1) ? BoxCompareY 
                                  : BoxCompareZ;
    // clang-format on

    // [start, end)
    size_t object_span = end - start;

    if (object_span == 1) {
      left_ = right_ = objects[start];
    } else if (object_span == 2) {
      left_ = objects[start];
      right_ = objects[start + 1];
    } else {
      std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

      auto mid = start + object_span / 2;
      left_ = make_shared<BvhNode>(objects, start, mid);
      right_ = make_shared<BvhNode>(objects, mid, end);
    }

    bbox_ = AABB(left_->BoundingBox(), right_->BoundingBox());
  }

  bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
    if (!bbox_.Hit(r, ray_t))
      return false;

    bool hit_left = left_->Hit(r, ray_t, rec);
    bool hit_right = right_->Hit(r, Interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

    return hit_left || hit_right;
  }

  AABB BoundingBox() const override { return bbox_; }

private:
  static bool BoxCompare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b,
                         int axis_index) {
    auto a_axis_interval = a->BoundingBox().AxisInterval(axis_index);
    auto b_axis_interval = b->BoundingBox().AxisInterval(axis_index);
    return a_axis_interval.min < b_axis_interval.min;
  }

  static bool BoxCompareX(const shared_ptr<Hittable>&a, const shared_ptr<Hittable>& b) {
    return  BoxCompare(a, b, 0);
  }

  static bool BoxCompareY(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b) {
    return  BoxCompare(a, b, 1);
  }

  static bool BoxCompareZ(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b) {
    return  BoxCompare(a, b, 2);
  }

private:
  AABB bbox_;
  shared_ptr<Hittable> left_;
  shared_ptr<Hittable> right_;
};
