#include "point_box_squared_distance.h"

double point_box_squared_distance(
  const Eigen::RowVector3d & query,
  const BoundingBox & box)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here
  Eigen::RowVector3d c = 0.5*(box.max_corner + box.min_corner);
  Eigen::RowVector3d dims = (box.max_corner - box.min_corner);

  // Computes distance for special case where point is inside box
  // if (box.min_corner[0] <= query[0] <= box.max_corner[0] && 
  //     box.min_corner[1] <= query[1] <= box.max_corner[1] &&
  //     box.min_corner[2] <= query[2] <= box.max_corner[2]) {
  //     return std::pow(std::min(
  //       std::min(
  //         std::min(
  //           std::min(
  //             std::min(
  //               std::abs(query[0] - box.min_corner[0]),
  //               std::abs(query[2] - box.max_corner[2])), 
  //               std::abs(query[2] - box.min_corner[2])), 
  //               std::abs(query[1] - box.max_corner[0])), 
  //               std::abs(query[1] - box.min_corner[1])), 
  //               std::abs(query[0] - box.max_corner[0])), 2);
  // }

  // Computes the component distances, evaluates to 0 if point is between box bounds for a component
  double xd = std::max(std::abs(query[0] - c[0]) - (dims[0] / 2), 0.0);
  double yd = std::max(std::abs(query[1] - c[1]) - (dims[1] / 2), 0.0);
  double zd = std::max(std::abs(query[2] - c[2]) - (dims[2] / 2), 0.0);

  // Compute distance
  return std::pow(xd, 2.0) + std::pow(yd, 2.0) + std::pow(zd, 2.0);
  ////////////////////////////////////////////////////////////////////////////
}
