#include "kinematics_jacobian.h"
#include "transformed_tips.h"
#include "copy_skeleton_at.h"
#include <iostream>

void kinematics_jacobian(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  Eigen::MatrixXd & J)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  J = Eigen::MatrixXd::Zero(b.size()*3,skeleton.size()*3);

  // The current tip positions
  Eigen::VectorXd base_x = transformed_tips(skeleton, b);

  double h = 0.00001; // Finite difference approximation step

  // The current angle vector a
  Eigen::VectorXd base_a;
  base_a.resize(skeleton.size() * 3);
  for (int j = 0; j < skeleton.size(); j++) {
    base_a[j * 3 + 0] = skeleton[j].xzx[0];
    base_a[j * 3 + 1] = skeleton[j].xzx[1];
    base_a[j * 3 + 2] = skeleton[j].xzx[2];
  }

  // Loop through every Euler angle
  for (int th = 0; th < skeleton.size(); th++) {

    for (int i = 0; i < 3; i++) {

      // Computes the 1 hot delta vector for this angle component's derivate approximation
      Eigen::VectorXd delta;
      delta.setZero(skeleton.size() * 3);
      delta[th * 3 + i] = 1;

      // da in the direction of angle th component i
      Eigen::VectorXd shifted_a = base_a + (h * delta);

      // Resulting dx after the da shift
      Eigen::VectorXd shifted_x = transformed_tips(copy_skeleton_at(skeleton, shifted_a), b);

      // Set the column of the dx/da for the current Euler angle
      J.col(th * 3 + i) = (shifted_x - base_x) / h;

    }

  }

  /////////////////////////////////////////////////////////////////////////////
}
