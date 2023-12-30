#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>

Eigen::Vector3d blinn_phong_shading(
  const Ray & ray,
  const int & hit_id, 
  const double & t,
  const Eigen::Vector3d & n,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector<std::shared_ptr<Light> > & lights)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  Material mat = *objects[hit_id].get()->material.get();

  Eigen::Vector3d final_color = (mat.ka.array() * Eigen::Vector3d(0.1, 0.1, 0.1).array()).matrix();
  // std::cout << "[" << mat.ka[0] << ", " << mat.ka[1] << ", " << mat.ka[2] << "]   [" 
  // << final_color[0] << ", " << final_color[1] << ", " << final_color[2] << "]\n";

  for (int i=0; i<lights.size(); i++) {
    // Get direction towards light at hit point
    Eigen::Vector3d light_dir;
    double max_t;
    Eigen::Vector3d query_point = ray.origin + ray.direction * t;
    lights[i].get()->direction(query_point, light_dir, max_t);

    // Get first intersection toward light to check for shadow
    // The 1e-8 prevents self-collisions due to float errors
    Ray to_light = {query_point, light_dir};
    int __dummy1;
    double light_t;
    Eigen::Vector3d __dummy2;
    bool is_blocked = first_hit(to_light, 1e-10, objects, __dummy1, light_t, __dummy2);

    // If light is not blocked
    if (! (is_blocked && light_t < max_t)) {
      // Compute half vector
      Eigen::Vector3d unnorm_h = light_dir.normalized() + (-ray.direction.normalized());
      Eigen::Vector3d h = unnorm_h.normalized();

      Eigen::Vector3d specular_col = std::pow(std::max(h.dot(n.normalized()), 0.0), mat.phong_exponent) * 
                    (mat.ks.array() * lights[i].get()->I.array()).matrix();
      Eigen::Vector3d shade_col = std::max(n.dot(light_dir), 0.0) * 
                    (mat.kd.array() * lights[i].get()->I.array()).matrix();

      final_color += specular_col;
      final_color += shade_col;
    }

  }

  return final_color;
  ////////////////////////////////////////////////////////////////////////////
}
