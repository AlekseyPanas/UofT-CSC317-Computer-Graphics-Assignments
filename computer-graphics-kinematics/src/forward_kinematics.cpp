#include "forward_kinematics.h"
#include "euler_angles_to_transform.h"
#include <functional> // std::function
#include <iostream>

void forward_kinematics(
  const Skeleton & skeleton,
  std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T)
{
  //std::cout << "forward_kinematics: START\n";

  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code

  // Fill T with identity transforms (initially)
  T.resize(skeleton.size(),Eigen::Affine3d::Identity());

  // Tracks whether a node has its transformation already computed and stored in T
  std::vector<bool> is_computed;
  is_computed.resize(T.size(), false);

  // Empty stack
  std::vector<int> stack;
  stack.resize(0);

  // Loop through every bone
  for (int i = 0; i < skeleton.size(); i++) {
    // Adds bone if not computed
    if (!is_computed[i]) {
      stack.push_back(i);
    }

    // Computes bone and all of its parents until stack empties (loop ensures that more bones get added if skeleton isnt finished)
    while(stack.size() > 0) {
      // Get bone at top of stack
      int cur_idx = stack[stack.size() - 1];
      int parent_idx = skeleton[cur_idx].parent_index;
      
      // If parent not evaluated, add parent to stack top and reboot loop
      if ((parent_idx != -1) && (!is_computed[parent_idx])) {
        stack.push_back(parent_idx);
      } 
      
      // Parent evaluated, means we can evaluate this node as well
      else {
        stack.pop_back(); // Removes this element cuz itll be computed now

        // Computes relative rotation sandwich
        Eigen::Affine3d relative_rot = skeleton[cur_idx].rest_T * 
                                       euler_angles_to_transform(skeleton[cur_idx].xzx) * 
                                       skeleton[cur_idx].rest_T.inverse();

        // If parent, apply parent trans after relative rot. Otherwise only relative rot
        if (parent_idx == -1) {
          T[cur_idx] = relative_rot;
        } else {
          T[cur_idx] = T[parent_idx] * relative_rot;
        }

        // Set computed
        is_computed[cur_idx] = true;
      }
    }
  }

  //std::cout << "forward_kinematics: END\n";
  /////////////////////////////////////////////////////////////////////////////
}
