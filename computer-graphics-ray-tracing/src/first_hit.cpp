#include "first_hit.h"

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  
  // Set the t value to a "rejected" t value to check for an unset t later
  t = min_t - 1;
  bool hit_ever = false;

  // Loop through all objects
  for (int i = 0; i < objects.size(); i++) {
    // compute intersection to object
    double first_t;
    Eigen::Vector3d normal;
    bool is_hit = objects[i].get()->intersect(ray, min_t, first_t, normal);
    
    // If the newfound t is better, OR, t was not yet set (determined by t < min_t)
    if (is_hit && (first_t < t || t < min_t)) {
      // Update the variables accordingly
      t = first_t;
      n = normal;
      hit_id = i;

      hit_ever = true;
    }
  }

  return hit_ever;
  ////////////////////////////////////////////////////////////////////////////
}
