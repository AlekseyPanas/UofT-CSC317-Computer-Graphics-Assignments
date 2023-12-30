#include "fast_mass_springs_precomputation_dense.h"
#include "signed_incidence_matrix_dense.h"
#include <Eigen/Dense>
#include <iostream>

bool fast_mass_springs_precomputation_dense(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXd & m,
  const Eigen::VectorXi & b,
  const double delta_t,
  Eigen::VectorXd & r,
  Eigen::MatrixXd & M,
  Eigen::MatrixXd & A,
  Eigen::MatrixXd & C,
  Eigen::LLT<Eigen::MatrixXd> & prefactorization)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  
  //std::cout << "precomp 1\n";

  // Incidence Matrix
  signed_incidence_matrix_dense(V.rows(), E, A);

  //std::cout << "precomp 2\n";

  // Mass matrix
  M = Eigen::MatrixXd::Zero(V.rows(), V.rows());
  for (int i = 0; i < V.rows(); i++) {
    M.row(i)[i] = m[i];
  }

  //std::cout << "precomp 3\n";

  // lengths
  r.resize(E.rows());
  for (int i = 0; i < E.rows(); i++) {
    r[i] = (V.row(E.row(i)[0]) - V.row(E.row(i)[1])).norm();
  }

  //std::cout << "precomp 4\n";

  // Selection Matrix
  C = Eigen::MatrixXd::Zero(b.rows(), V.rows());
  for (int i = 0; i < b.rows(); i++) {
    C.row(i)[b[i]] = 1;
  }

  //std::cout << "precomp 5\n";

  // Do the Q thingie ¯\_(ツ)_/¯
  double w = 1e10;
  Eigen::MatrixXd Q = (k * (A.transpose() * A)) + ((1 / (std::pow(delta_t, 2))) * M) + 
      ((w * C.transpose()) * C); // Quadratic penalty term

  /////////////////////////////////////////////////////////////////////////////
  prefactorization.compute(Q);
  return prefactorization.info() != Eigen::NumericalIssue;
}
