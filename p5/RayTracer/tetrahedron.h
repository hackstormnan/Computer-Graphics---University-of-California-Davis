#pragma once
#include "hittable.h"
#include <iostream>
class tetrahedron : public hittable {
public:
  tetrahedron() {}
  tetrahedron(point3 p_p0, point3 p_p1, point3 p_p2, point3 p_p3, shared_ptr<material> m)
    : p0(p_p0), p1(p_p1), p2(p_p2), p3(p_p3), mat_ptr(m)
  {
  }

  virtual bool
  hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

  vec3
  cal_normal(point3 v0, point3 v1, point3 v2) const;

  point3 p0;
  point3 p1;
  point3 p2;
  point3 p3;
  shared_ptr<material> mat_ptr;

  bool
  hit_one_face(point3 v0, point3 v1, point3 v2, const ray& r, double t_min, double t_max, hit_record& rec) const;

private:
  mutable double cur_t = infinity;
};

bool
tetrahedron::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
  cur_t = infinity;
  bool result0 = hit_one_face(p0, p1, p2, r, t_min, t_max, rec);
  bool result1 = hit_one_face(p3, p0, p2, r, t_min, t_max, rec);
  bool result2 = hit_one_face(p3, p2, p1, r, t_min, t_max, rec);
  bool result3 = hit_one_face(p3, p1, p0, r, t_min, t_max, rec);
  if (!result0 && !result1 && !result2 && !result3) 
    return false;
  return true;
}

vec3
tetrahedron::cal_normal(point3 v0,
                     point3 v1,
                     point3 v2) const // https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
{
  vec3 normal = vec3(0, 0, 0);

  vec3 U = v1 - v0; // (Triangle.p2 minus Triangle.p1)
  vec3 V = v2 - v0; // (Triangle.p3 minus Triangle.p1)

  normal[0] = U[1] * V[2] - U[2] * V[1]; // Set Normal.x to(multiply U.y by V.z) minus(multiply U.z by V.y)
  normal[1] = U[2] * V[0] - U[0] * V[2]; // Set Normal.y to(multiply U.z by V.x) minus(multiply U.x by V.z)
  normal[2] = U[0] * V[1] - U[1] * V[0]; // Set Normal.z to(multiply U.x by V.y) minus(multiply U.y by V.x)

  return normal.normalize();
}

inline bool
tetrahedron::hit_one_face(point3 v0, point3 v1, point3 v2, const ray& r, double t_min, double t_max, hit_record& rec)
  const
{
  vec3 n = cal_normal(v0, v1, v2);

  // parallel with the plane                //first check if triangle intesact with the plane
  double dDotN = dot(r.direction(), n);
  if (dDotN == 0.0f) {
    return false;
  }

  double t = dot(v1 - r.origin(), n) / dDotN;

  if (t <= t_min || t >= t_max) {
    return false;
  }

  point3 q = r.at(t); // if hit plane, check if the hit point is inside triangle

  if (dot(cross((v1 - v0), (q - v0)), n) < 0) {
    return false;
  }
  if (dot(cross((v2 - v1), (q - v1)), n) < 0) {
    return false;
  }
  if (dot(cross((v0 - v2), (q - v2)), n) < 0) {
    return false;
  }

  if (t > cur_t) {
    return false;
  }
  cur_t = t;
  rec.t = t;
  rec.p = r.at(rec.t);
  rec.set_face_normal(r, n);
  rec.mat_ptr = mat_ptr;
  return true;
}
