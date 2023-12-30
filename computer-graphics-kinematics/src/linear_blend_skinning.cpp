#include "linear_blend_skinning.h"
#include <iostream>

void linear_blend_skinning(
  const Eigen::MatrixXd & V,
  const Skeleton & skeleton,
  const std::vector<Eigen::Affine3d,Eigen::aligned_allocator<Eigen::Affine3d> > & T,
  const Eigen::MatrixXd & W,
  Eigen::MatrixXd & U)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  //std::cout << "LBS: START\n";

  U.resize(V.rows(), 3);

  // Loop through vertices
  for (int v = 0; v < V.rows(); v++) {
    Eigen::Vector3d new_v = Eigen::Vector3d(0, 0, 0);
    Eigen::Vector3d cur_vertex = V.row(v).transpose();

    // Loop through skeleton
    for (int b = 0; b < skeleton.size(); b++) {
      int widx = skeleton[b].weight_index;

      if (widx != -1) {
        // Accumulate weighed transform with this bone
        new_v += W.row(v)[widx] * (T[b] * (cur_vertex));
      }
      
    }

    if (new_v != Eigen::Vector3d(0, 0, 0)) {
      U.row(v) = new_v;
    } else {
      U.row(v) = V.row(v);
    }
    
  }

  //U = V; // For testing

  //std::cout << "LBS: END\n";
  /////////////////////////////////////////////////////////////////////////////
}
