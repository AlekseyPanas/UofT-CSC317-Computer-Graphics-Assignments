#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:

  rgb = Eigen::Vector3d(0,0,0);

  // Caps recursion depth
  if (num_recursive_calls >= 4) {
    return false;
  }

  // compute first hit, end if false
  Eigen::Vector3d n;
  int hit_id;
  double t;
  bool was_hit = first_hit(ray, min_t, objects, hit_id, t, n);
  if (!was_hit) {return false;}

  // Compute the color as a result of specular reflection and lambertian shading, add it to total col
  rgb += blinn_phong_shading(ray, hit_id, t, n, objects, lights);

  // Compute reflection ray
  Eigen::Vector3d reflected_direction = reflect(ray.direction.normalized(), n);
  Ray reflected_ray = {t * ray.direction + ray.origin, reflected_direction};

  // Compute color at reflection ray and add it if found
  Eigen::Vector3d reflected_color;
  bool success = raycolor(reflected_ray, 1e-8, objects, lights, num_recursive_calls + 1, reflected_color);
  if (success) {
    rgb += (objects[hit_id].get()->material.get()->km.array() * reflected_color.array()).matrix();
  }
  
  return true;
  ////////////////////////////////////////////////////////////////////////////
}
