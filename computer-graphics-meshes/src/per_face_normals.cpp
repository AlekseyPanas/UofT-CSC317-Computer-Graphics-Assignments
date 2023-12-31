#include "per_face_normals.h"
#include "triangle_area_normal.h"
#include <iostream>

void per_face_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code:
  N.resize(F.rows(), 3);

  for (int i = 0; i < F.rows(); i++) {
    N.row(i) = triangle_area_normal(V.row(F.row(i)[0]), V.row(F.row(i)[1]), V.row(F.row(i)[2])).normalized();
  }
  ////////////////////////////////////////////////////////////////////////////
}
