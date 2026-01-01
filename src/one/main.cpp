#include "camera.h"
#include "common.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
  HittableList world;

  auto material_ground = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<Lambertian>(color(0.1, 0.2, 0.5));
  auto material_left   = make_shared<Dielectric>(1.50);
  auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
  auto material_right  = make_shared<Metal>(color(0.8, 0.6, 0.2), 1.0);

  world.Add(make_shared<Sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
  world.Add(make_shared<Sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
  world.Add(make_shared<Sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
  world.Add(make_shared<Sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
  world.Add(make_shared<Sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

  Camera cam;

  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 50;

  cam.vfov     = 20;
  cam.lookfrom = point3(-2, 2,  1);
  cam.lookat   = point3( 0, 0, -1);
  cam.vup      = vec3(   0, 1,  0);

  cam.Render(world);
}
