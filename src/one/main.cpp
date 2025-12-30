
#include "color.h"
#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

color RayColor(const Ray& r, const HittableList& world) {
  HitRecord rec;
  if (world.Hit(r, 0, kInfinity, rec)) {
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

int main() {
  // ----------------------------------------------------------------------------: image

  double aspect_ratio = 16.0 / 9.0;
  int image_width = 400;

  // Calculate the image height, and ensure that it's at least 1.
  int image_height = int(image_width / aspect_ratio);
  image_height = (image_height < 1) ? 1 : image_height;

  // ----------------------------------------------------------------------------: world

  HittableList world;
  world.Add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
  world.Add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

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

      color pixel_color = RayColor(ray, world);
      write_color(std::cout, pixel_color);
    }
  }
  std::clog << "\rDone.                 \n";
}
