#include "line_search.h"
#include <iostream>

double line_search(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const Eigen::VectorXd & z,
  const Eigen::VectorXd & dz,
  const double max_step)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  double cur_step = max_step;

  double best_f = f(z); // Start at base error

  while (cur_step > 1e-4) {
    Eigen::VectorXd moved_z = z - (cur_step * dz); // Compute stepped Z and project
    proj_z(moved_z);
    
    double new_f = f(moved_z); // Compute error after step, return if better than base error
    if (new_f < best_f) {
      return cur_step;
    }

    cur_step = cur_step / 2; // Cut step
  }

  return cur_step;
  /////////////////////////////////////////////////////////////////////////////
}
