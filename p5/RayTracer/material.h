#ifndef MATERIAL_H
#define MATERIAL_H
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

#include "rtweekend.h"

struct hit_record;
class hittable;

/* forward declaration for function defined in main.cc */
color
trace_ray(const hittable& world, const light_list& lights, const ray& r, int depth);

/* forward declaration for function defined in main.cc */
bool
trace_shadow(const hittable& world, const ray& r);

class material {
public:
  /* Note: here we remove the 'scatter' function, and replace it with a more general 'shade' function
  virtual bool
  scatter(const ray& in_r, const hit_record& in_rec, color& out_attenuation, ray& out_scattered) const {};
  */

  // this function computes the color of a ray
  virtual bool
  shade(const ray& r,
        const hit_record& rec,
        const hittable& world,
        const light_list& lights,
        const int depth,
        color& output) const = 0;
};

class lambertian : public material {    //no reflection
public:
  lambertian(const color& a) : albedo(a) {}

  virtual bool
  shade(const ray& r,
        const hit_record& rec,
        const hittable& world,
        const light_list& lights,
        const int depth,
        color& output) const override
  {
    auto scatter_direction = rec.normal/* + random_unit_vector()*/;

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    output = albedo * trace_ray(world, lights, ray(rec.p, scatter_direction), depth - 1);
    return true;
  }

public:
  color albedo;
};

class dielectric : public material {                    //transparent
public:
  dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  virtual bool
  shade(const ray& r,
        const hit_record& rec,
        const hittable& world,
        const light_list& lights,
        const int depth,
        color& output) const
  {
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = unit_vector(r.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;

    /*if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
      direction = reflect(unit_direction, rec.normal);
    else*/
      direction = refract(unit_direction, rec.normal, refraction_ratio);

    output = color(1.0, 1.0, 1.0) * trace_ray(world, lights, ray(rec.p, direction), depth - 1);
    return true;
  }

public:
  double ir; // Index of Refraction

private:
  static double
  reflectance(double cosine, double ref_idx)
  {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

class metal : public material {
public:
  metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

  virtual bool
  shade(const ray& r,
        const hit_record& rec,
        const hittable& world,
        const light_list& lights,
        const int depth,
        color& output) const
  {
    vec3 reflected = reflect(unit_vector(r.direction()), rec.normal);
    ray scattered = ray(rec.p, reflected /*+ fuzz * random_in_unit_sphere()*/);
    if (dot(scattered.direction(), rec.normal) > 0) {
      output = albedo * trace_ray(world, lights, scattered, depth - 1);
      return true;
    }
    return false;
  }

public:
  color albedo;
  double fuzz;
};

class phong : public material {
public:
  phong(color diffuse, color specular, int glossiness) 
  : diffuse_ (diffuse), specular_(specular), glossiness_(glossiness){}

  virtual bool
  shade(const ray& r,
        const hit_record& rec,
        const hittable& world,
        const light_list& lights,
        const int depth,
        color& output) const
  {
    point3 hit_point = rec.p;
    color a = color(0,0,0);
    color d = color(0,0,0);
    color s = color(0,0,0);
    for (light* l : lights) {
      if (l->is_ambient() == true) { // add ambient color
        a += diffuse_ * l->illuminance(hit_point);
      }
      else {
        ray ray_to_light(hit_point, l->direction(hit_point));
        if (trace_shadow(world, ray_to_light)) { // if not ambient light, then check if can reach light
          //diffuse
          vec3 norm = rec.normal;
          vec3 lightDir = l->direction(hit_point);
          d += dot(norm, lightDir) * diffuse_ * l->illuminance(hit_point); // if exposed to light source

          //specular reflect
          vec3 viewDir = (r.origin() - hit_point).normalize();
          vec3 reflectDir = reflect(-lightDir, norm);
          double spec = pow(dot(viewDir, reflectDir), glossiness_);
          s += spec * specular_;
        }
      }
    }
    vec3 reflected = reflect(unit_vector(r.direction()), rec.normal);
    output = (a + d + s) * trace_ray(world, lights, ray(rec.p, reflected), depth - 1);
    return true;
  }
  color diffuse_;
  color specular_;
  int glossiness_;
};

#endif
