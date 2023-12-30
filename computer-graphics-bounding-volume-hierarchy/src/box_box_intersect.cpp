#include "box_box_intersect.h"
bool box_box_intersect(
  const BoundingBox & A,
  const BoundingBox & B)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  return std::max(A.min_corner[0], B.min_corner[0]) <= std::min(A.max_corner[0], B.max_corner[0]) &&
  std::max(A.min_corner[1], B.min_corner[1]) <= std::min(A.max_corner[1], B.max_corner[1]) &&
  std::max(A.min_corner[2], B.min_corner[2]) <= std::min(A.max_corner[2], B.max_corner[2]);

  return false;
  ////////////////////////////////////////////////////////////////////////////
}

