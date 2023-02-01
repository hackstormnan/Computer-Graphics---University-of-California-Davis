#pragma once
#include "hittable.h"

class plane : public hittable {
public:
  plane() {}

  plane(point3 cen, vec3 n, shared_ptr<material> m) : point(cen), normal(n), mat_ptr(m){};

  virtual bool
  hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
  point3 point;
  vec3 normal;
  shared_ptr<material> mat_ptr;
};

bool
plane::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    //parallel with the plane
  double dDotN = dot(r.direction(), normal);
  if (dDotN == 0.0f) {
    return false;
  }

  double t = dot(point - r.origin(), normal) / dDotN;

  if (t <= t_min || t >= t_max) {
    return false;
  }

  rec.t = t;
  rec.p = r.at(rec.t);
  rec.set_face_normal(r, normal);
  rec.mat_ptr = mat_ptr;

  return true;
}
