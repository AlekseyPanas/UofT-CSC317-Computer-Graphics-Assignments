#include "ray_intersect_box.h"
#include <iostream>
#include <limits>

bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t,
  const double max_t)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:

  double tmins[3];
  double tmaxs[3];

  // Compute tmins and tmaxs for each of x,y,z
  for (int i = 0; i < 3; i++) {
    if (ray.direction[i] == 0) {
      if (! (box.min_corner[i] <= ray.origin[i] <= box.max_corner[i])) {
        return false;
      } else {
        tmins[i] = -std::numeric_limits<double>::infinity();
        tmaxs[i] = std::numeric_limits<double>::infinity();
      }
    }
    else if (ray.direction[i] > 0) {
      tmins[i] = (box.min_corner[i] - ray.origin[i]) / ray.direction[i];
      tmaxs[i] = (box.max_corner[i] - ray.origin[i]) / ray.direction[i];
    } else if (ray.direction[i] < 0) {
      tmins[i] = (box.max_corner[i] - ray.origin[i]) / ray.direction[i];
      tmaxs[i] = (box.min_corner[i] - ray.origin[i]) / ray.direction[i];
    }
  }

  // The t value after which all mins have been crossed
  double entry_point = std::max(std::max(tmins[0], tmins[1]), tmins[2]);
  // The t value after which at least one max has been crossed (meaning we left the box)
  double exit_point = std::min(std::min(tmaxs[0], tmaxs[1]), tmaxs[2]);

  // We must fully enter before we exit the box to have a collision
  // Also check that if an intersection did happen that it was within min_t and max_t
  // std::cout << "Box COLLISION:" << entry_point << "        " << exit_point << "           " << min_t << "         " << max_t << "\n";
  if (entry_point <= exit_point && (
    ((min_t <= exit_point) && (entry_point <= min_t)) || 
    ((max_t <= exit_point) && (entry_point <= max_t)) ||
    ((min_t <= entry_point) && (exit_point <= max_t))
    )) {
      // std::cout << "We got collision" << "\n";
    return true;
  }

  return false;
  ////////////////////////////////////////////////////////////////////////////
}
