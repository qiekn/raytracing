#pragma once

#include "color.h"
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
        vec3 pixel_center = pixel00_loc_ + (i * pixel_delta_u_) + (j * pixel_delta_v_);
        vec3 ray_direction = pixel_center - camera_center_;
        Ray ray(camera_center_, ray_direction);

        color pixel_color = RayColor(ray, world);
        write_color(std::cout, pixel_color);
      }
    }
    std::clog << "\rDone.                 \n";
  }

private:
  void Initialize() {
    image_height_ = int(image_width / aspect_ratio);
    image_height_ = (image_height_ < 1) ? 1 : image_height_;

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

  color RayColor(const Ray& r, const Hittable& world) const {
    HitRecord rec;
    if (world.Hit(r, Interval(0, kInfinity), rec)) {
      return 0.5 * (rec.normal + color(1, 1, 1));
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

// ----------------------------------------------------------------------------: data
public:
  double aspect_ratio = 16.0 / 9.0;
  int    image_width  = 400;

private:
  // Calculate the image height, and ensure that it's at least 1.
  int    image_height_;   // Rendered iamge height
  point3 camera_center_;         // Camera center
  point3 pixel00_loc_;    // Location of pixel 0, 0 (upper left)
  vec3   pixel_delta_u_;  // Offset to pixel to the right
  vec3   pixel_delta_v_;  // Offset to pixel below
};
