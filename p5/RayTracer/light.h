#ifndef LIGHT_H
#define LIGHT_H
//===========================================================================//
//                                                                           //
// Copyright(c) ECS 175 (2020)                                               //
// University of California, Davis                                           //
// MIT Licensed                                                              //
//                                                                           //
//===========================================================================//

#include "rtweekend.h"

#include <vector>

class light {
public:
  virtual ~light() = default;

  // compute the 'I'
  virtual color
  illuminance(const point3& p /* shading position */) const = 0;

  // compute the light direction
  virtual vec3
  direction(const point3& p /* shading position */) const = 0;

  virtual bool
  is_ambient() const
  {
    return false;
  }
};

typedef std::vector<light*> light_list;

class ambient_light : public light {
public:
  ambient_light(color c) 
      : col(c) {}
  virtual color
  illuminance(const vec3& p) const override
  {
    return col;
  }

  // compute the light direction
  virtual vec3
  direction(const vec3& p) const override
  {
    return vec3(0,0,0);
  }

  virtual bool
  is_ambient() const override
  {
    return true;
  }
  color col;
};

class directional_light : public light {
public:
  directional_light(color c, vec3 d) 
      : col (c), dir(d) {}
  virtual color
  illuminance(const vec3& p) const override
  {
    return col;
  }

  virtual vec3
  direction(const vec3& p) const override
  {
    return -dir.normalize();
  }

  color col;
  vec3 dir;
};

class point_light : public light {
public:
  point_light(color c, point3 p) 
      : col(c), position(p){}
  virtual color
  illuminance(const vec3& p) const override
  {
    return col;
  }

  virtual vec3
  direction(const vec3& p) const override
  {
    return (position - p).normalize();
  }
  point3 position;
  color col;
};

#endif // LIGHT_H
