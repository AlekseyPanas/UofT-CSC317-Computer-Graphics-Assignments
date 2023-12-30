#include "end_effectors_objective_and_gradient.h"
#include "transformed_tips.h"
#include "kinematics_jacobian.h"
#include "copy_skeleton_at.h"
#include <iostream>

void end_effectors_objective_and_gradient(
  const Skeleton & skeleton,
  const Eigen::VectorXi & b,
  const Eigen::VectorXd & xb0,
  std::function<double(const Eigen::VectorXd &)> & f,
  std::function<Eigen::VectorXd(const Eigen::VectorXd &)> & grad_f,
  std::function<void(Eigen::VectorXd &)> & proj_z)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  f = [&](const Eigen::VectorXd & A)->double
  {
    // Get the end effector bone tips of a skeleton transformed into a frame defined by A
    Eigen::VectorXd tips = transformed_tips(copy_skeleton_at(skeleton, A), b);

    // Return squared norm
    return (tips - xb0).squaredNorm();
  };


  grad_f = [&](const Eigen::VectorXd & A)->Eigen::VectorXd
  {
    // Compute tips
    Eigen::VectorXd tips = transformed_tips(copy_skeleton_at(skeleton, A), b);

    // Compute kinematic jacobian
    Eigen::MatrixXd J;
    kinematics_jacobian(skeleton, b, J);

    // dE/dtheta
    return 2 * (J.transpose() * (tips - xb0));
  };


  proj_z = [&](Eigen::VectorXd & A)
  {
    assert(skeleton.size()*3 == A.size());

    // Loop through skeleton
    for (int i = 0; i < skeleton.size(); i++) {
      // Clamp the current 3 coords in A to be between the bounds
      Eigen::Vector3d clamped = Eigen::Vector3d(A[i * 3 + 0], A[i * 3 + 1], 
                                  A[i * 3 + 2]).cwiseMax(skeleton[i].xzx_min).cwiseMin(skeleton[i].xzx_max);
      // Reassign back to A
      A[i * 3 + 0] = clamped[0];
      A[i * 3 + 1] = clamped[1];
      A[i * 3 + 2] = clamped[2];
    }
  };
  /////////////////////////////////////////////////////////////////////////////
}
