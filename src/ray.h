#pragma once

#include "vec3.h"

class Ray {
public:
  Ray() {}

  Ray(const point3& origin, const vec3& direction)
      : Ray(origin, direction, 0){}

  Ray(const point3& origin, const vec3& direction, double time)
      : orig_(origin), dir_(direction), time_(time) {}

  const point3& origin() const { return orig_; }

  const vec3& direction() const { return dir_; }

  double time() const { return time_; }

  point3 at(double t) const { return orig_ + t * dir_; }

private:
  point3 orig_;
  vec3 dir_;
  double time_;
};
