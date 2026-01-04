#include "bvh.h"
#include "camera.h"
#include "color.h"
#include "common.h"
#include "constant_medium.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"
#include "vec3.h"

// ----------------------------------------------------------------------------: scenes
void BouncingSpheres() {
  HittableList world;

  auto checker = make_shared<CheckerTexture>(0.32, color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
  world.Add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));

  auto ground_material = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
  world.Add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = RandomDouble();
      point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<Material> Sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          Sphere_material = make_shared<Lambertian>(albedo);
          auto center2 = center + vec3(0, RandomDouble(0, 0.5), 0);
          world.Add(make_shared<Sphere>(center, center2, 0.2, Sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = RandomDouble(0, 0.5);
          Sphere_material = make_shared<Metal>(albedo, fuzz);
          world.Add(make_shared<Sphere>(center, 0.2, Sphere_material));
        } else {
          // glass
          Sphere_material = make_shared<Dielectric>(1.5);
          world.Add(make_shared<Sphere>(center, 0.2, Sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<Dielectric>(1.5);
  world.Add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
  world.Add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
  world.Add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

  // world = HittableList(make_shared<BvhNode>(world));

  Camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(0.70, 0.90, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  cam.Render(world);
}

void CheckeredSpheres() {
  HittableList world;

  auto checker = make_shared<CheckerTexture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

  world.Add(make_shared<Sphere>(point3(0, -10, 0), 10, make_shared<Lambertian>(checker)));
  world.Add(make_shared<Sphere>(point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));

  world = HittableList(make_shared<BvhNode>(world));

  Camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(0.70, 0.90, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;
  cam.Render(world);
}

void Earch() {
  auto earth_texture = make_shared<ImageTexture>("earthmap.jpg");
  auto earth_surface = make_shared<Lambertian>(earth_texture);
  auto globe = make_shared<Sphere>(point3(0, 0, 0), 2, earth_surface);

  Camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(0.70, 0.90, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(0, 0, 12);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.Render(HittableList(globe));
}

void PerlinSphere() {
  HittableList world;

  auto pertext = make_shared<NoiseTexture>(4);
  world.Add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
  world.Add(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

  Camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(0.70, 0.90, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(12, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.Render(world);
}

void Quads() {
  HittableList world;

  // Materials
  auto left_red = make_shared<Lambertian>(color(1.0, 0.2, 0.2));
  auto back_green = make_shared<Lambertian>(color(0.2, 1.0, 0.2));
  auto right_blue = make_shared<Lambertian>(color(0.2, 0.2, 1.0));
  auto upper_orange = make_shared<Lambertian>(color(1.0, 0.5, 0.0));
  auto lower_teal = make_shared<Lambertian>(color(0.2, 0.8, 0.8));

  // Quads
  world.Add(make_shared<Quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
  world.Add(make_shared<Quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
  world.Add(make_shared<Quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
  world.Add(make_shared<Quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
  world.Add(make_shared<Quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

  Camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(0.70, 0.90, 1.00);

  cam.vfov = 80;
  cam.lookfrom = point3(0, 0, 9);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.Render(world);
}

void EasterEggs() {
  HittableList world;

  // Materials
  auto red = make_shared<Lambertian>(color(1.0, 0.2, 0.2));
  auto green = make_shared<Lambertian>(color(0.2, 1.0, 0.2));
  auto blue = make_shared<Lambertian>(color(0.2, 0.2, 1.0));
  auto orange = make_shared<Lambertian>(color(1.0, 0.5, 0.0));
  auto teal = make_shared<Lambertian>(color(0.2, 0.8, 0.8));

  // Primitives: 2x2 grid layout
  world.Add(make_shared<Quad>(point3(-2.0, 0.1, 0), vec3(1.8, 0, 0), vec3(0, 1.8, 0), red));
  world.Add(
      make_shared<Triangle>(point3(0.2, 0.1, 0), vec3(1.8, 0, 0), vec3(0, 1.8, 0), green));
  world.Add(
      make_shared<Ellipse>(point3(-1.1, -1.1, 0), vec3(0.9, 0, 0), vec3(0, 0.9, 0), blue));
  world.Add(make_shared<Annulus>(point3(1.1, -1.1, 0), vec3(0.9, 0, 0), vec3(0, 0.9, 0), 0.5,
                                 orange));

  Camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(0.70, 0.90, 1.00);

  cam.vfov = 20;
  cam.lookfrom = point3(0, 0, 12);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.Render(world);
}

void SimpleLights() {
  HittableList world;

  auto pertext = make_shared<NoiseTexture>(4);
  world.Add(make_shared<Sphere>(point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
  world.Add(make_shared<Sphere>(point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

  auto difflight = make_shared<DiffuseLight>(color(4, 4, 4));
  world.Add(make_shared<Sphere>(point3(0, 7, 0), 2, difflight));
  world.Add(make_shared<Quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

  Camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = color(0, 0, 0);

  cam.vfov = 20;
  cam.lookfrom = point3(26, 3, 6);
  cam.lookat = point3(0, 2, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.Render(world);
}

void CornelBox() {
  HittableList world;

  auto red = make_shared<Lambertian>(color(.65, .05, .05));
  auto white = make_shared<Lambertian>(color(.73, .73, .73));
  auto green = make_shared<Lambertian>(color(.12, .45, .15));
  auto light = make_shared<DiffuseLight>(color(15, 15, 15));

  // clang-format off
  world.Add(make_shared<Quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
  world.Add(make_shared<Quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
  world.Add(make_shared<Quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
  world.Add(make_shared<Quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
  world.Add(make_shared<Quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
  world.Add(make_shared<Quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));
  // clang-format on

  shared_ptr<Hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
  box1 = make_shared<RotateY>(box1, 15);
  box1 = make_shared<Translate>(box1, vec3(265, 0, 295));
  world.Add(box1);

  shared_ptr<Hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
  box2 = make_shared<RotateY>(box2, -18);
  box2 = make_shared<Translate>(box2, vec3(130, 0, 65));
  world.Add(box2);

  world = HittableList(make_shared<BvhNode>(world));

  Camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 600;
  cam.samples_per_pixel = 200;
  cam.max_depth = 50;
  cam.background = color(0, 0, 0);

  cam.vfov = 40;
  cam.lookfrom = point3(278, 278, -800);
  cam.lookat = point3(278, 278, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.Render(world);
}

void CornellSmoke() {
  HittableList world;

  auto red = make_shared<Lambertian>(color(.65, .05, .05));
  auto white = make_shared<Lambertian>(color(.73, .73, .73));
  auto green = make_shared<Lambertian>(color(.12, .45, .15));
  auto light = make_shared<DiffuseLight>(color(7, 7, 7));

  world.Add(make_shared<Quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
  world.Add(make_shared<Quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
  world.Add(make_shared<Quad>(point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
  world.Add(make_shared<Quad>(point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
  world.Add(make_shared<Quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
  world.Add(make_shared<Quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

  shared_ptr<Hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
  box1 = make_shared<RotateY>(box1, 15);
  box1 = make_shared<Translate>(box1, vec3(265, 0, 295));

  shared_ptr<Hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
  box2 = make_shared<RotateY>(box2, -18);
  box2 = make_shared<Translate>(box2, vec3(130, 0, 65));

  world.Add(make_shared<ConstantMedium>(box1, 0.01, color(0, 0, 0)));
  world.Add(make_shared<ConstantMedium>(box2, 0.01, color(1, 1, 1)));

  Camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 600;
  cam.samples_per_pixel = 200;
  cam.max_depth = 50;
  cam.background = color(0, 0, 0);

  cam.vfov = 40;
  cam.lookfrom = point3(278, 278, -800);
  cam.lookat = point3(278, 278, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.Render(world);
}

// ----------------------------------------------------------------------------: main
int main() {
  // clang-format off
  switch (9) {
    // case 1
    // BVH on:  Render time: 52.8369s
    // BVH off: Render time: 236.774s
    case 1: BouncingSpheres(); break;
    case 2: CheckeredSpheres(); break;
    case 3: Earch(); break;
    case 4: PerlinSphere(); break;
    case 5: Quads(); break;
    case 6: EasterEggs(); break;
    case 7: SimpleLights(); break;
    case 8: CornelBox(); break; // Render time: 311.929s
    case 9: CornellSmoke(); break;
  }
  // clang-format on
}
