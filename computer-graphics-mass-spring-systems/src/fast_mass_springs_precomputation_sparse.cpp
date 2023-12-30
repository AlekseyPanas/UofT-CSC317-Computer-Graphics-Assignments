#include "fast_mass_springs_precomputation_sparse.h"
#include "signed_incidence_matrix_sparse.h"
#include <vector>

bool fast_mass_springs_precomputation_sparse(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXd & m,
  const Eigen::VectorXi & b,
  const double delta_t,
  Eigen::VectorXd & r,
  Eigen::SparseMatrix<double>  & M,
  Eigen::SparseMatrix<double>  & A,
  Eigen::SparseMatrix<double>  & C,
  Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > & prefactorization)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code

  // Incidence Matrix
  signed_incidence_matrix_sparse(V.rows(), E, A);

  // Mass matrix
  std::vector<Eigen::Triplet<double> > m_triplets;
  for (int i = 0; i < V.rows(); i++) {
    m_triplets.emplace_back(i, i, m[i]);
  }
  M.resize(V.rows(), V.rows());
  M.setFromTriplets(m_triplets.begin(), m_triplets.end());

  // lengths
  r.resize(E.rows());
  for (int i = 0; i < E.rows(); i++) {
    r[i] = (V.row(E.row(i)[0]) - V.row(E.row(i)[1])).norm();
  }

  // Selection Matrix
  std::vector<Eigen::Triplet<double> > c_triplets;
  for (int i = 0; i < b.rows(); i++) {
    c_triplets.emplace_back(i, b[i], 1);
  }
  C.resize(b.rows(), V.rows());
  C.setFromTriplets(c_triplets.begin(), c_triplets.end());

  // QQQQQQQQQQQQQ
  
  // std::vector<Eigen::Triplet<double> > ijv;
  const int n = V.rows();

  // for (int i = 0; i < n; i++) {
  //   ijv.emplace_back(i,i,1);
  // }

  double w = 1e10;
  Eigen::SparseMatrix<double> Q(n,n);
  Q = (k * (A.transpose() * A)) + ((1 / (std::pow(delta_t, 2))) * M) + 
      ((w * C.transpose()) * C); // Quadratic penalty term


  /////////////////////////////////////////////////////////////////////////////
  prefactorization.compute(Q);
  return prefactorization.info() != Eigen::NumericalIssue;
}
