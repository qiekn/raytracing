
#include <cmath>
#include "color.h"
#include "ray.h"
#include "vec3.h"

// ray   : P(t) = O + tD
// sphere: |C - P| = r
double HitSphere(const vec3& center, double radius, const Ray& r) {
  vec3 oc = center - r.origin();
  double a = dot(r.direction(), r.direction());
  double b = -2.0 * dot(r.direction(), oc);
  double c = dot(center - r.origin(), oc) - radius * radius;

  double discriminant = b * b - 4 * a * c;  // 判别式
  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-b - std::sqrt(discriminant)) / (2.0 * a);
  }
  return (discriminant >= 0);
}

color RayColor(const Ray& r) {
  auto t = HitSphere(point3(0, 0, -1), 0.5, r);  // Red sphere
  if (t > 0.0) {
    vec3 normal = unit_vector(r.at(t) - vec3(0,0,-1));
    return 0.5 * color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
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

int main() {
  // ----------------------------------------------------------------------------: image

  double aspect_ratio = 16.0 / 9.0;
  int image_width = 400;

  // Calculate the image height, and ensure that it's at least 1.
  int image_height = int(image_width / aspect_ratio);
  image_height = (image_height < 1) ? 1 : image_height;

  // ----------------------------------------------------------------------------: camera

  double focal_length = 1.0;
  double viewport_height = 2.0;
  double viewport_width = viewport_height * (double(image_width) / image_height);
  vec3 camera_center = point3(0, 0, 0);

  // Calculate the vectors across the horiziontal and down the vertical viewport edges.
  vec3 viewport_u = vec3(viewport_width, 0, 0);
  vec3 viewport_v = vec3(0, -viewport_height, 0);

  // Calculate the horizontal and vertical **delta vectors** from pixel to pixel.
  vec3 pixel_delta_u = viewport_u / image_width;
  vec3 pixel_delta_v = viewport_v / image_height;

  // Calculate the localtion of the upper left pixel.
  vec3 viewport_upper_left =
      camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
  vec3 pixel00_pos = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  // ----------------------------------------------------------------------------: render

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; j++) {
    std::clog << "\rScanline remaining: " << (image_height - j) << ' ' << std::flush;
    for (int i = 0; i < image_width; i++) {
      vec3 pixel_center = pixel00_pos + (i * pixel_delta_u) + (j * pixel_delta_v);
      vec3 ray_direction = pixel_center - camera_center;
      Ray ray(camera_center, ray_direction);

      color pixel_color = RayColor(ray);
      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                 \n";
}
