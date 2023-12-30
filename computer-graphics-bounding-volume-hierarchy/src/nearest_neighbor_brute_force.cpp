#include "nearest_neighbor_brute_force.h"
#include <limits>// std::numeric_limits<double>::infinity();

void nearest_neighbor_brute_force(
  const Eigen::MatrixXd & points,
  const Eigen::RowVector3d & query,
  int & I,
  double & sqrD)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  I = -1;
  sqrD = std::numeric_limits<double>::infinity();

  for (int i = 0; i < points.rows(); i++) {
    double new_sqrD = (points.row(i) - query).array().pow(2).matrix().sum();
    if (new_sqrD < sqrD) {
      sqrD = new_sqrD;
      I = i;
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
