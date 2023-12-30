#include "copy_skeleton_at.h"
Skeleton copy_skeleton_at(
  const Skeleton & skeleton, 
  const Eigen::VectorXd & A)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  Skeleton copy;

  for (int i = 0; i < skeleton.size(); i++) {
    copy.push_back(skeleton[i]);
    copy[i].xzx = Eigen::Vector3d(A[i * 3 + 0], A[i * 3 + 1], A[i * 3 + 2]);
  }

  return copy;
  /////////////////////////////////////////////////////////////////////////////
}
