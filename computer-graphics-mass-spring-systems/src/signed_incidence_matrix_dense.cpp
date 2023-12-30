#include "signed_incidence_matrix_dense.h"
#include <iostream>

void signed_incidence_matrix_dense(
  const int n,
  const Eigen::MatrixXi & E,
  Eigen::MatrixXd & A)
{
  //////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  A = Eigen::MatrixXd::Zero(E.rows(),n);

  // Loop edges
  for (int i = 0; i < E.rows(); i++) {
    A.row(i)[E.row(i)[0]] = 1;
    A.row(i)[E.row(i)[1]] = -1;
  }

  // std::cout << E << "\n\n";
  // std::cout << A << "\n\n";
  //////////////////////////////////////////////////////////////////////////////
}
