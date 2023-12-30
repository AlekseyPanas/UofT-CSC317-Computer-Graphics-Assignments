#include "transformed_tips.h"
#include "forward_kinematics.h"
#include <iostream>

Eigen::VectorXd transformed_tips(
  const Skeleton & skeleton, 
  const Eigen::VectorXi & b)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  //std::cout << "transformed_tips: START\n";

  // Get forward kinematics affines
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > rest_to_frame;
  forward_kinematics(skeleton, rest_to_frame);

  Eigen::VectorXd output;
  output.resize(b.size() * 3);

  // Loop through indexes
  for (int i = 0; i < b.size(); i++) {
    Bone cur_bone = skeleton[b[i]];

    // get the tip in bone space (length along x-axis), move to rest space, then use T to move to frame space
    Eigen::Vector3d tip_pos = rest_to_frame[b[i]] * cur_bone.rest_T * Eigen::Vector3d(cur_bone.length, 0.0, 0.0);

    // Set output
    output[i * 3 + 0] = tip_pos[0];
    output[i * 3 + 1] = tip_pos[1];
    output[i * 3 + 2] = tip_pos[2];
  }

  //std::cout << "transformed_tips: END\n";

  return output;
  /////////////////////////////////////////////////////////////////////////////
}
