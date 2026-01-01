#pragma once

#include <iostream>
#include "color.h"
#include "common.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class Camera {
// ----------------------------------------------------------------------------: methods
public:
  void Render(const Hittable& world) {
    Initialize();

    std::cout << "P3\n" << image_width << ' ' << image_height_ << "\n255\n";

    for (int j = 0; j < image_height_; j++) {
      std::clog << "\rScanline remaining: " << (image_height_ - j) << ' ' << std::flush;
      for (int i = 0; i < image_width; i++) {
        color pixel_color(0,0,0);
        for (int sample = 0; sample < samples_per_pixel; sample++) {
          Ray r = GetRay(i, j);
          pixel_color += RayColor(r, max_depth, world);
        }
        write_color(std::cout, pixel_samples_scale_ * pixel_color);
      }
    }
    std::clog << "\rDone.                 \n";
  }

private:
  void Initialize() {
    image_height_ = int(image_width / aspect_ratio);
    image_height_ = (image_height_ < 1) ? 1 : image_height_;

    pixel_samples_scale_ = 1.0 / samples_per_pixel;

    camera_center_ = point3(0, 0, 0);

    // Determin viewport dimensions.
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(image_width) / image_height_);

    // Calculate the vectors across the horiziontal and down the vertical viewport edges.
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical **delta vectors** from pixel to pixel.
    pixel_delta_u_ = viewport_u / image_width;
    pixel_delta_v_ = viewport_v / image_height_;

    // Calculate the localtion of the upper left pixel.
    vec3 viewport_upper_left =
        camera_center_ - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
  }

  Ray GetRay(int i, int j) const {
    // Construct a camera ray originating from the origin and directed 
    // at randomly sampled point around the pixel location i, j

    vec3 offset = SampleSquare();
    vec3 pixel_sample = pixel00_loc_ + 
                      ((i + offset.x()) * pixel_delta_u_) +
                      ((j + offset.y()) * pixel_delta_v_);
    point3 ray_origin = camera_center_;
    vec3 ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
  }

  color RayColor(const Ray& r, int depth, const Hittable& world) const {
    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0)
      return color(0, 0, 0);

    HitRecord rec;
    if (world.Hit(r, Interval(0, kInfinity), rec)) {
      vec3 direction = random_on_hemispshere(rec.normal);
      return 0.5 * RayColor(Ray(rec.p, direction), depth - 1, world);
    }

    vec3 unit_direction = unit_vector(r.direction());
    // -1..1 -> 0..2 -> 0..1
    double a = 0.5 * (unit_direction.y() + 1.0);
    // A simple gradient (white to blue)
    // a=0       : white
    // a-> 0 ~ 1 : blend white & blup
    // a=1       : blue
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
  }

  // Returns the vector to a ramdom point in the [-0.5, -0.5]~[0.5, 0.5] unit squre.
  vec3 SampleSquare() const  {
    return vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
  }

// ----------------------------------------------------------------------------: data
public:
  double aspect_ratio = 1.0;   // Ratio of image width over height
  int image_width = 100;       // Rendered image width in pexel count
  int samples_per_pixel = 10;  // Count of random samples for each pixel
  int max_depth = 10;          // Maximum number of ray bounces into scene

private:
  // Calculate the image height, and ensure that it's at least 1.
  int image_height_;            // Rendered iamge height
  double pixel_samples_scale_;  // Color scale factor for a sum of pixel samples
  point3 camera_center_;        // Camera center
  point3 pixel00_loc_;          // Location of pixel 0, 0 (upper left)
  vec3 pixel_delta_u_;          // Offset to pixel to the right
  vec3 pixel_delta_v_;          // Offset to pixel below
};
