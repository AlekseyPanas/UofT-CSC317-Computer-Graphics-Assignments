#include "triangle_area_normal.h"
#include <Eigen/Geometry>
#include <iostream>

Eigen::RowVector3d triangle_area_normal(
  const Eigen::RowVector3d & a, 
  const Eigen::RowVector3d & b, 
  const Eigen::RowVector3d & c)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code:

  Eigen::RowVector3d t1 = b - a;
  Eigen::RowVector3d t2 = c - a;

  // Magnitude is determinant of t1, t2, which is area of parallellogram, twice triangle
  Eigen::RowVector3d cross = t1.cross(t2);
  return cross / 2;
  ////////////////////////////////////////////////////////////////////////////
}
