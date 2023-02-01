//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "light.h"
#include "material.h"
#include "rtweekend.h"

//hittables
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include "triangle.h"
#include "tetrahedron.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "xmlload.h"
#include <iostream>

#if ENABLE_TBB
#include "tbb/blocked_range.h"
#include "tbb/tbb.h"
#endif

hittable_list
random_scene()
{
  hittable_list world;

  auto ground_material = make_shared<phong>(color(0.9, 0.9, 0.9), color(1, 1, 1), 256);
  world.add(make_shared<plane>(point3(0, 0, 0), vec3(0, 1, 0), ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.8) {
        shared_ptr<material> sphere_material;
       if (choose_mat < 0.8)
        {
          // phong
          auto diffuse = color::random() * color::random();
          sphere_material = make_shared<phong>(diffuse, color(1,1,1), 256);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
       else if (choose_mat < 0.6) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else if (choose_mat < 0.4) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }
  auto material1 = make_shared<dielectric>(1.5); // transparent
  auto material2 = make_shared<phong>(color(0.0, 0.9, 0.9), color(1, 1, 1), 256); // no reflection
  auto material3 = make_shared<metal>(color(0.6, 0.7, 0.8), 0.0);

  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
  world.add(make_shared<sphere>(point3(0, 1, -2.5), 1.0, material2));
  world.add(make_shared<sphere>(point3(0, 1, 2.5), 1.0, material3));

  return world;
}

color
trace_ray(const hittable& world, const light_list& lights, const ray& r, int depth)
{
  hit_record rec;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0)
    return color(0, 0, 0);

  // If the ray hits anything
  if (world.hit(r, /* t min */ 0.001, /* t max */ infinity, /* hit record */ rec)) {
    color output;
    if (rec.mat_ptr->shade(r, rec, world, lights, depth, output)) 
    {
      return output;
    }
    return color(0, 0, 0); // if doesn't hit anything
  }
  // Miss routine
  vec3 unit_direction = unit_vector(r.direction());             //sky box
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

bool
trace_shadow(const hittable& world, const ray& r)
{
  hit_record rec;
  if (world.hit(r, /* t min */ 0.001, /* t max */ infinity, /* hit record */ rec)) {
    std::shared_ptr<dielectric> ptr = std::dynamic_pointer_cast<dielectric>(rec.mat_ptr);
    if (ptr != nullptr) {   //light should penatrate glass material
      return true;
    }
    return false;
  }
  return true; // if in shadow, or false if not in shadow
}

int main() {
  // Image
  LoadScene("inputs/scene1.xml");

  int image_width = image_width_;
  int image_height = image_height_;
  int samples_per_pixel = samples_per_pixel_;
  int max_depth = max_depth_;

  double aspect_ratio = (double)image_width / (double)image_height;

  // World
  //hittable_list world = random_scene();
  hittable_list world = world_;

  // Camera
  camera cam(/* look from */ lookfrom_,
             /* look at */ lookat_,
             /* up vector */ up_,
             fov_,
             aspect_ratio,
             /* aperture */ aperture_,
             /* distance to focus */ 10.0);

  // Light
  light_list lights = lights_;

  color * framebuffer = new color[image_height * image_width];

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(image_width, image_height, "Ray Tracing", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK)
    return false;

  Shader shader("Shader/Default.shader");
  float point[] = { 0, 0, 0 };

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.Bind();
    glBindVertexArray(VAO);

  // Render
  auto render_pixel_simple = [&](int i, int j) {
    auto u = (float)i / (image_width - 1);
    auto v = (float)j / (image_height - 1);
    ray r = cam.get_ray(u, v);
    color pixel_color = trace_ray(world, lights, r, max_depth);
    framebuffer[i + j * image_width] = normalize_color(pixel_color, 1);
  };

  auto render_pixel_anti_aliased = [&](int i, int j) {
    color pixel_color(0, 0, 0);
    for (int s = 0; s < samples_per_pixel; ++s) {
      // anti aliasing
      auto u = (i + random_double()) / (image_width - 1);
      auto v = (j + random_double()) / (image_height - 1);
      ray r = cam.get_ray(u, v);
      pixel_color += trace_ray(world, lights, r, max_depth);
    }
    framebuffer[i + j * image_width] = normalize_color(pixel_color, samples_per_pixel);
  };

  // Colors for different pixels can be computed in parallel.
#if ENABLE_TBB
  tbb::parallel_for(0, image_height, [&](int j) {
    tbb::parallel_for(0, image_width, [&](int i) {
      render_pixel_simple(i, j); /* compute the color of one pixel */
      // render_pixel_anti_aliased(i, j);
    });
  });
#else
  for (int j = 0; j < image_height; ++j) {
    std::cerr << "\rScanlines remaining: " << j << "/" << image_height - 1 << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      render_pixel_simple(i, j);
      // render_pixel_anti_aliased(i, j);
    }
  }
  std::cerr << std::endl << "Done." << std::endl;
#endif

  // Write as PPM to standard output

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for (int j = image_height - 1; j >= 0; --j) {
    for (int i = 0; i < image_width; ++i) {
      const color& p = framebuffer[i + j * image_width];
       // Write the translated [0,255] value of each color component.
       //std::cout << static_cast<int>(256 * clamp(p.x(), 0.0, 0.999)) << ' '
       //         << static_cast<int>(256 * clamp(p.y(), 0.0, 0.999)) << ' '
       //         << static_cast<int>(256 * clamp(p.z(), 0.0, 0.999)) << '\n';
      float x = float(i) * 2 / image_width - 1.0f; 
      float y = float(j) * 2 / image_height - 1.0f;
      shader.SetUniformVec2("screenPos", glm::vec2(x, y));
      shader.SetUniformVec3("vertexColor", glm::vec3(p.x(), p.y(), p.z()));
      glDrawArrays(GL_POINTS, 0, 1);
     }
  }

  glfwSwapBuffers(window);
  glfwPollEvents();
}
  glfwTerminate();

  delete[] framebuffer;
  return 0;
}
