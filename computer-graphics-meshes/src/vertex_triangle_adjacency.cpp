#include "vertex_triangle_adjacency.h"

void vertex_triangle_adjacency(
  const Eigen::MatrixXi & F,
  const int num_vertices,
  std::vector<std::vector<int> > & VF)
{
  VF.resize(num_vertices);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  for (int f = 0; f < F.rows(); f++) {
    VF[F.row(f)[0]].push_back(f);
    VF[F.row(f)[1]].push_back(f);
    VF[F.row(f)[2]].push_back(f);
  }

  ////////////////////////////////////////////////////////////////////////////
}

