#include "PointLight.h"

void PointLight::direction(
  const Eigen::Vector3d & q, Eigen::Vector3d & d, double & max_t) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:

  // Difference between points gives direction
  Eigen::Vector3d unnorm = (p + (-q));
  max_t = unnorm.norm();
  d = unnorm.normalized();

  ////////////////////////////////////////////////////////////////////////////
}
