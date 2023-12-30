#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:

  // Denominator of plane collision formula from lecture
  double denominator = (normal.dot(ray.direction));
  
  // Indicates parallel plane, return no collision
  if (denominator == 0) {
    return false;
  }

  // Compute t value of collision
  t = (normal.dot(point) - normal.dot(ray.origin)) / denominator;

  // Return if the t value was valid, and set normal
  if (t < min_t) {
    return false;
  }
  n = normal;
  return true;
  ////////////////////////////////////////////////////////////////////////////
}

