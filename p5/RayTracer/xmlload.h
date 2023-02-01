//-----------------------------------------------------------------------------
///
/// \file       xmlload.h
/// \author     Qi WU
/// \version    2.0
/// \date       August 28, 2017
///
/// \brief Example source for CS 6620 - University of Utah.
///
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
///
/// \file       xmlload.cpp
/// \author     Cem Yuksel (www.cemyuksel.com)
/// \version    11.0
/// \date       November 6, 2017
///
/// \brief Example source for CS 6620 - University of Utah.
///
//-----------------------------------------------------------------------------

#pragma once
#ifndef XMLLOAD_H
#define XMLLOAD_H

#include "hittable_list.h"
#include "light.h"
#include "material.h"
#include "vec3.h"

#include "material.h"
#include "light.h"
#include "cube.h"
#include "triangle.h"
#include "sphere.h"
#include "plane.h"
#include "tetrahedron.h"

#include "tinyxml.h"

//-----------------------------------------------------------------------------

#ifdef WIN32
#define COMPARE(a, b) (_stricmp(a, b) == 0)
#else
#define COMPARE(a, b) (strcasecmp(a, b) == 0)
#endif

#define PRINTF printf

#include <map>

//-----------------Variables with default values----------------------------------
int image_width_ = 640;
int image_height_ = 360;
int samples_per_pixel_ = 10;
int fov_ = 20;
int max_depth_ = 10;
double aperture_ = 0.0001;

vec3 lookfrom_ = point3(13, 3, 0);
vec3 lookat_ = point3(0, 1, 0);
vec3 up_ = vec3(0, 1, 0);

hittable_list world_;
std::map<std::string, shared_ptr<material>> materials_;
light_list lights_;

void
LoadObjects(TiXmlElement* element, int level = 0);

void
LoadMaterial(TiXmlElement* element);

void
LoadLight(TiXmlElement* element);

void
LoadCamera(TiXmlElement* element);

//-----------------------------------------------------------------------------

void
ReadVector(TiXmlElement* element, vec3& v);

void
ReadColor(TiXmlElement* element, color& c);

void
ReadFloat(TiXmlElement* element, float& f, const char* name = "value");

void
ReadString(TiXmlElement* element, std::string & str, const char* name = "value");

//-----------------------------------------------------------------------------

// TODO you can change the function signature
inline int
LoadScene(const char* filename) 
{
  TiXmlDocument doc(filename);
  if (!doc.LoadFile()) {
    PRINTF("Failed to load the file \"%s\"\n", filename);
    return 0;
  }

  TiXmlElement* xml = doc.FirstChildElement("xml");
  if (!xml) {
    PRINTF("No \"xml\" tag found.\n");
    return 0;
  }

  TiXmlElement* scene = xml->FirstChildElement("scene");
  if (!scene) {
    PRINTF("No \"scene\" tag found.\n");
    return 0;
  }

  LoadLight(scene);
  LoadMaterial(scene);  
  LoadObjects(scene);

  TiXmlElement* cam = xml->FirstChildElement("camera");
  if (!cam) {
    PRINTF("No \"camera\" tag found.\n");
    return 0;
  }
  LoadCamera(cam);

  return 1;
}
//-----------------------------------------------------------------------------

inline void
LoadObjects(TiXmlElement* element, int level)
{
  element = element->FirstChildElement("object");

  while (element != nullptr) {
    // name
    std::string name_str;
    const char* name = element->Attribute("name");
    PRINTF("Object [");
    if (name) {
      PRINTF("%s", name);
      name_str = name;
    }
    PRINTF("]");

    // type
    const char* type = element->Attribute("type");
    if (type) {
      if (COMPARE(type, "sphere")) {
        PRINTF(" - Sphere object\n");
        point3 center(1, 1, 1);
        float radius = 0;
        std::string mat = "";
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "center")) {
            ReadVector(child, center);
            PRINTF("   center %f %f %f\n", center.x(), center.y(), center.z());
          }
          else if (COMPARE(child->Value(), "radius")) {
            ReadFloat(child, radius);
            PRINTF("   radius %f\n", radius);
          }
          else if (COMPARE(child->Value(), "material")) {
            ReadString(child, mat);
            PRINTF("   material %s\n", mat.c_str());
          }
        }
        auto ob = make_shared<sphere>(center, radius, materials_[mat]);
        world_.add(ob);
      }
      else if (COMPARE(type, "plane")) {
          PRINTF(" - Plane object\n");
          point3 point(1, 1, 1);
          vec3 normal(1, 1, 1);
          std::string mat = "";
          for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
            if (COMPARE(child->Value(), "point")) {
              ReadVector(child, point);
              PRINTF("   point %f %f %f\n", point.x(), point.y(), point.z());
            }
            if (COMPARE(child->Value(), "normal")) {
              ReadVector(child, normal);
              PRINTF("   normal %f %f %f\n", normal.x(), normal.y(), normal.z());
            }
            else if (COMPARE(child->Value(), "material")) {
              ReadString(child, mat);
              PRINTF("   material %s\n", mat.c_str());
            }
          }
          auto ob = make_shared<plane>(point, normal, materials_[mat]);
          world_.add(ob);
        }
      else if (COMPARE(type, "triangle")) {
        PRINTF(" - Triangle object\n");
        point3 p0(1, 1, 1);
        point3 p1(1, 1, 1);
        point3 p2(1, 1, 1);
        std::string mat = "";
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "p0")) {
            ReadVector(child, p0);
            PRINTF("   p0 %f %f %f\n", p0.x(), p0.y(), p0.z());
          }
          else if(COMPARE(child->Value(), "p1"))
          {
            ReadVector(child, p1);
            PRINTF("   p1 %f %f %f\n", p1.x(), p1.y(), p1.z());
          }
          else if(COMPARE(child->Value(), "p2"))
          {
            ReadVector(child, p2);
            PRINTF("   p2 %f %f %f\n", p2.x(), p2.y(), p2.z());
          }
          else if (COMPARE(child->Value(), "material")) {
            ReadString(child, mat);
            PRINTF("   material %s\n", mat.c_str());
          }
        }
        auto ob = make_shared<triangle>(p0, p1, p2, materials_[mat]);
        world_.add(ob);
      }
      else if (COMPARE(type, "tetrahedron")) {
        PRINTF(" - Tetrahedron object\n");
        point3 p0(1, 1, 1);
        point3 p1(1, 1, 1);
        point3 p2(1, 1, 1);
        point3 p3(1, 1, 1);
        std::string mat = "";
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "p0")) {
            ReadVector(child, p0);
            PRINTF("   p0 %f %f %f\n", p0.x(), p0.y(), p0.z());
          }
          else if (COMPARE(child->Value(), "p1")) {
            ReadVector(child, p1);
            PRINTF("   p1 %f %f %f\n", p1.x(), p1.y(), p1.z());
          }
          else if (COMPARE(child->Value(), "p2")) {
            ReadVector(child, p2);
            PRINTF("   p2 %f %f %f\n", p2.x(), p2.y(), p2.z());
          }
          else if (COMPARE(child->Value(), "p3")) {
            ReadVector(child, p3);
            PRINTF("   p3 %f %f %f\n", p3.x(), p3.y(), p3.z());
          }
          else if (COMPARE(child->Value(), "material")) {
            ReadString(child, mat);
            PRINTF("   material %s\n", mat.c_str());
          }
        }
        auto ob = make_shared<tetrahedron>(p0, p1, p2, p3, materials_[mat]);
        world_.add(ob);
      }
      else if (COMPARE(type, "cube")) {
        PRINTF(" - Cube object\n");
        point3 t_min(1, 1, 1);
        point3 t_max(1, 1, 1);
        std::string mat = "";
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "min")) {
            ReadVector(child, t_min);
            PRINTF("   t_min %f %f %f\n", t_min.x(), t_min.y(), t_min.z());
          }
          else if (COMPARE(child->Value(), "max")) {
            ReadVector(child, t_max);
            PRINTF("   max %f %f %f\n", t_max.x(), t_max.y(), t_max.z());
          }
          else if (COMPARE(child->Value(), "material")) {
            ReadString(child, mat);
            PRINTF("   material %s\n", mat.c_str());
          }
        }
        auto ob = make_shared<cube>(t_min, t_max, materials_[mat]);
        world_.add(ob);
      }
    }
    element = element->NextSiblingElement("object");
  }
}

inline void
LoadMaterial(TiXmlElement* element)
{
  element = element->FirstChildElement("material");
  
  while (element != nullptr) {
    // name
    std::string name_str;
    const char* name = element->Attribute("name");
    PRINTF("Material [");
    if (name) {
      PRINTF("%s", name);
      name_str = name;
    }
    PRINTF("]");

    // type
    const char* type = element->Attribute("type");
    if (type) {
      if (COMPARE(type, "phong")) {
        PRINTF(" - Phong material\n");
        color diffuse(1, 1, 1);
        color specular(1, 1, 1);
        float glossiness = 0;
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "diffuse")) {
            ReadColor(child, diffuse);
            PRINTF("   diffuse %f %f %f\n", diffuse.x(), diffuse.y(), diffuse.z());
          }
          else if (COMPARE(child->Value(), "specular")) {
            ReadColor(child, specular);
            PRINTF("   specular %f %f %f\n", specular.x(), specular.y(), specular.z());
          }
          else if (COMPARE(child->Value(), "glossiness"))
          {
            ReadFloat(child, glossiness);
            PRINTF("   glossiness %f\n", glossiness);
          }
        }
        auto p = make_shared<phong>(diffuse, specular, (int)glossiness);
        materials_.insert(std::pair<std::string, shared_ptr<material>>(name_str, p));
      }
      else if (COMPARE(type, "lambertian")) {
        PRINTF(" - Lambertian material\n");
        color albedo(1, 1, 1);
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "albedo")) {
            ReadColor(child, albedo);
            PRINTF("   albedo %f %f %f\n", albedo.x(), albedo.y(), albedo.z());
          }
        }
        auto p = make_shared<lambertian>(albedo);
        materials_.insert(std::pair<std::string, shared_ptr<material>>(name_str, p));
      }
      else if (COMPARE(type, "dielectric")) {
        PRINTF(" - Dielectric material\n");
        float index_of_refraction = 0;
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "index_of_refraction")) {
            ReadFloat(child, index_of_refraction);
            PRINTF("   index_of_refraction %f\n", index_of_refraction);
          }
        }
        auto p = make_shared<dielectric>((double)index_of_refraction);
        materials_.insert(std::pair<std::string, shared_ptr<material>>(name_str, p));
      }
      else if (COMPARE(type, "metal")) {
        PRINTF(" - Metal material\n");
        color albedo(1, 1, 1);
        float fuzz = 0;
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "albedo")) {
            ReadColor(child, albedo);
            PRINTF("   albedo %f %f %f\n", albedo.x(), albedo.y(), albedo.z());
          }
          else if (COMPARE(child->Value(), "fuzz")) {
            ReadFloat(child, fuzz);
            PRINTF("   fuzz %f\n", fuzz);
          }
        }
        auto p = make_shared<metal>(albedo, (double)fuzz);
        materials_.insert(std::pair<std::string, shared_ptr<material>>(name_str, p));
      }
    }
    element = element->NextSiblingElement("material");
  }
}

inline void
LoadLight(TiXmlElement* element)
{
  element = element->FirstChildElement("light");

  while (element != nullptr) {
    // name
    const char* name = element->Attribute("name");
    PRINTF("Light [");
    if (name)
      PRINTF("%s", name);
    PRINTF("]");

    // type
    const char* type = element->Attribute("type");
    if (type) {
      if (COMPARE(type, "ambient")) {
        PRINTF(" - Ambient Light\n");
        float c = 0;
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "color")) {
            ReadFloat(child, c);
            PRINTF("   color %f\n", c);
          }
        }
        ambient_light* ambient = new ambient_light(color(c, c, c));
        lights_.push_back(ambient);
      }
      else if (COMPARE(type, "directional")) {
        PRINTF(" - Directional Light\n");

        vec3 v(1, 1, 1);
        float c = 0;
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "color")) {
            ReadFloat(child, c);
            PRINTF("   color %f\n", c);
          }
          else if (COMPARE(child->Value(), "direction")) {
            ReadVector(child, v);

            PRINTF("   direction %f %f %f\n", v.x(), v.y(), v.z());
          }
        }
        directional_light* directional = new directional_light(color(c, c, c), v);
        lights_.push_back(directional);
      }
      else if (COMPARE(type, "point")) {
        PRINTF(" - Point Light\n");

        vec3 v(0, 0, 0);
        float c = 0;
        float f = 0;
        for (TiXmlElement* child = element->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
          if (COMPARE(child->Value(), "color")) {
            ReadFloat(child, c);

            PRINTF("   color %f\n", c);
          }
          else if (COMPARE(child->Value(), "position")) {
            ReadVector(child, v);

            PRINTF("   position %f %f %f\n", v.x(), v.y(), v.z());
          }
        }
        point_light* point = new point_light(color(c, c, c), v);
        lights_.push_back(point);
      }
    }
    element = element->NextSiblingElement("light");
  }
}

inline void
LoadCamera(TiXmlElement* cam)
{
  TiXmlElement* camChild = cam->FirstChildElement();
  while (camChild) {
    if (COMPARE(camChild->Value(), "lookfrom"))
      ReadVector(camChild, lookfrom_);
    else if (COMPARE(camChild->Value(), "lookat"))
      ReadVector(camChild, lookat_);
    else if (COMPARE(camChild->Value(), "upvector"))
      ReadVector(camChild, up_);
    else if (COMPARE(camChild->Value(), "image-width"))
      camChild->QueryIntAttribute("value", &image_width_);
    else if (COMPARE(camChild->Value(), "image-height"))
      camChild->QueryIntAttribute("value", &image_height_);
    else if (COMPARE(camChild->Value(), "samples-per-pixel"))
      camChild->QueryIntAttribute("value", &samples_per_pixel_);
    else if (COMPARE(camChild->Value(), "max-depth"))
      camChild->QueryIntAttribute("value", &max_depth_);
    else if (COMPARE(camChild->Value(), "fov"))
      camChild->QueryIntAttribute("value", &fov_);

    camChild = camChild->NextSiblingElement();
  }
}

//-----------------------------------------------------------------------------

inline void
ReadVector(TiXmlElement* element, vec3& v)
{
  double x = (double)v.x();
  double y = (double)v.y();
  double z = (double)v.z();
  element->QueryDoubleAttribute("x", &x);
  element->QueryDoubleAttribute("y", &y);
  element->QueryDoubleAttribute("z", &z);
  v = vec3(x, y, z);
  float f = 1;
  ReadFloat(element, f);
  v *= f;
}

//-----------------------------------------------------------------------------

inline void
ReadColor(TiXmlElement* element, color& c)
{
  double r = (double)c.x();
  double g = (double)c.y();
  double b = (double)c.z();
  element->QueryDoubleAttribute("r", &r);
  element->QueryDoubleAttribute("g", &g);
  element->QueryDoubleAttribute("b", &b);
  c = color(r, g, b);

  float f = 1;
  ReadFloat(element, f);
  c *= f;
}

//-----------------------------------------------------------------------------

inline void
ReadFloat(TiXmlElement* element, float& f, const char* name)
{
  double d = (double)f;
  element->QueryDoubleAttribute(name, &d);
  f = (float)d;
}

inline void
ReadString(TiXmlElement* element, std::string & str, const char* name)
{
  str = element->Attribute(name);
}

#endif // XMLLOAD_H
