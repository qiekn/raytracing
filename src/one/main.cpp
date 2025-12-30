
#include "camera.h"
#include "common.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

int main() {
  HittableList world;

  world.Add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
  world.Add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

  Camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;

  cam.Render(world);
}
