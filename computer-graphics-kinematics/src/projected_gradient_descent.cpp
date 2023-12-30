#include "projected_gradient_descent.h"
#include "line_search.h"
#include <iostream>

void projected_gradient_descent(
  const std::function<double(const Eigen::VectorXd &)> & f,
  const std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  const std::function<void(Eigen::VectorXd &)> & proj_z,
  const int max_iters,
  Eigen::VectorXd & z)
{
  /////////////////////////////////////////////////////////////////////////////
  // Add your code here

  // Perform iterations
  for (int i = 0; i < max_iters; i++) {

    // Get gradient
    Eigen::VectorXd dz = grad_f(z);

    // Get best step for descent
    double alpha = line_search(f, proj_z, z, dz, 32);

    // Update z: Compute step and project
    z = z - (alpha * dz);
    proj_z(z);

  }
  /////////////////////////////////////////////////////////////////////////////
}
