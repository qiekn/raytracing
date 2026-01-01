
#include <cmath>
#include "camera.h"
#include "common.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
  HittableList world;

  auto R = std::cos(kPi / 4);

  auto material_left = make_shared<Lambertian>(color(0, 0, 1));
  auto material_right = make_shared<Lambertian>(color(1, 0, 0));

  world.Add(make_shared<Sphere>(point3(-R, 0, -1), R, material_left));
  world.Add(make_shared<Sphere>(point3(R, 0, -1), R, material_right));

  Camera cam;
  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 50;
  cam.vfov              = 90;

  cam.Render(world);
}
