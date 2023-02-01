#ifndef AABB_H
#define AABB_H
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

#include "hittable.h"
class cube : public hittable {
public:
  cube() {}
  cube(const point3& a, const point3& b, shared_ptr<material> m) 
      :minimum(a), maximum(b), mat_ptr(m) {}
  
  point3
  min() const
  {
    return minimum;
  }
  point3
  max() const
  {
    return maximum;
  }

  virtual bool
  hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
  {
    double start_min = t_min;
    double t0x = (minimum.x() - r.origin().x()) / r.direction().x();
    double t1x = (maximum.x() - r.origin().x()) / r.direction().x(); 
    double t0y = (minimum.y() - r.origin().y()) / r.direction().y(); 
    double t1y = (maximum.y() - r.origin().y()) / r.direction().y(); 
    double t0z = (minimum.z() - r.origin().z()) / r.direction().z(); 
    double t1z = (maximum.z() - r.origin().z()) / r.direction().z();

    vec3 ray_sign;
    ray_sign[0] = r.direction().x() > 0 ? 1 : -1;
    ray_sign[1] = r.direction().y() > 0 ? 1 : -1;
    ray_sign[2] = r.direction().z() > 0 ? 1 : -1;
      for (int i = 0; i < 3; i++) {
      double t0 = fmin((minimum[i] - r.origin()[i]) / r.direction()[i], (maximum[i] - r.origin()[i]) / r.direction()[i]);
      double t1 = fmax((minimum[i] - r.origin()[i]) / r.direction()[i], (maximum[i] - r.origin()[i]) / r.direction()[i]);
      
      t_min = fmax(t0, t_min);
      t_max = fmin(t1, t_max);
      if (t_max <= t_min)
        return false;
      }
      vec3 normal;
      double t = t_min;
      if (t_min == start_min) {     // the origal is inside
        t = t_max;
      }
      if (t == t0x) { normal = vec3(-1, 0, 0); }    //check which face is onhit
      if (t == t1x) { normal = vec3(1, 0, 0);  }
      if (t == t0y) { normal = vec3(0, -1, 0); }
      if (t == t1y) { normal = vec3(0, 1, 0);  }
      if (t == t0z) { normal = vec3(0, 0, -1); }
      if (t == t1z) { normal = vec3(0, 0, 1);  }
    rec.t = t;
    rec.p = r.at(rec.t);
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;
   
    return true;
  }

  double
  area() const
  {
    auto a = maximum.x() - minimum.x();
    auto b = maximum.y() - minimum.y();
    auto c = maximum.z() - minimum.z();
    return 2 * (a * b + b * c + c * a);
  }

  int
  longest_axis() const
  {
    auto a = maximum.x() - minimum.x();
    auto b = maximum.y() - minimum.y();
    auto c = maximum.z() - minimum.z();
    if (a > b && a > c)
      return 0;
    else if (b > c)
      return 1;
    else
      return 2;
  }

public:
  point3 minimum;
  point3 maximum;
  shared_ptr<material> mat_ptr;
};

cube
surrounding_box(cube box0, cube box1, shared_ptr<material> m)
{
  vec3 small(
    fmin(box0.min().x(), box1.min().x()), fmin(box0.min().y(), box1.min().y()), fmin(box0.min().z(), box1.min().z()));

  vec3 big(
    fmax(box0.max().x(), box1.max().x()), fmax(box0.max().y(), box1.max().y()), fmax(box0.max().z(), box1.max().z()));

  return cube(small, big, m);
}

#endif
