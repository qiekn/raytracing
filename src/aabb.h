#pragma once

#include "interval.h"
#include "ray.h"
#include "vec3.h"

class AABB {
public:
  // ----------------------------------------------------------------------------: constructors
  AABB() {}  // The default AABB is empty, since intervals are empty by default.

  AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {
    PadToMinmums();
  }

  AABB(const point3& a, const point3& b) {
    // Treat the two points a and b as extrema for the bounding box,
    // so we don't require a particular min/max coordinate order.
    x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
    y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
    z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

    PadToMinmums();
  }

  AABB(const AABB& box0, const AABB& box1) {
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
  }

  // ----------------------------------------------------------------------------: methods
  // clang-format off

  // Return the interval corresponding to the given axis index:
  // 0 -> x axis, 1 -> y axis, 2 -> z axis.
  const Interval& AxisInterval(int n) const {
    if (n == 1) return y;
    if (n == 2) return z;
    return x;
  }

  bool Hit(const Ray& r, Interval ray_t) const {
    const point3& ray_orig = r.origin();
    const vec3& ray_dir = r.direction();

    for (int axis = 0; axis < 3; axis++) {
      https://tinyurl.com/2t5s7ptj
      const Interval& ax = AxisInterval(axis);
      const double ad_inv = 1.0 / ray_dir[axis];

      auto t0 = (ax.min - ray_orig[axis]) * ad_inv;
      auto t1 = (ax.max - ray_orig[axis]) * ad_inv;

      if (t0 < t1) {
        if (t0 > ray_t.min) ray_t.min = t0;
        if (t1 < ray_t.max) ray_t.max = t1;
      } else {
        if (t1 > ray_t.min) ray_t.min = t1;
        if (t0 < ray_t.max) ray_t.max = t0;
      }

      if (ray_t.max <= ray_t.min) return false;
    }
    return true;
  }

  // Return the index of the longest axis of the bounding box
  int LongestAxis() const {
    double x_size = x.Size();
    double y_size = y.Size();
    double z_size = z.Size();

    if (x_size > y_size && x_size > z_size)
        return 0;
    if (y_size > z_size)
        return 1;
    return 2;
  }

private:
  // Adjust the AABB so that no side is narrower than some delta, padding if necessary.
  void PadToMinmums() {
    double delta = 0.0001;
    if (x.Size() < delta) x = x.Expand(delta);
    if (y.Size() < delta) y = y.Expand(delta);
    if (z.Size() < delta) z = z.Expand(delta);
  }

// ----------------------------------------------------------------------------: data
public:
  Interval x, y, z; // Three one-dimensional intervals representing the AABB extents on each axis.
  static const AABB empty, universe;
  // clang-format on
};

const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

AABB operator+(const AABB& bbox, const vec3& offset) {
  return AABB(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

AABB operator+(const vec3& offset, const AABB& bbox) {
  return bbox + offset;
}
