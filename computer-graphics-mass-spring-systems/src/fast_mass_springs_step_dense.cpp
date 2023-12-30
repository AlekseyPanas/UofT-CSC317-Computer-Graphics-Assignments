#include "fast_mass_springs_step_dense.h"
#include <igl/matlab_format.h>
#include <iostream>

void fast_mass_springs_step_dense(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & E,
  const double k,
  const Eigen::VectorXi & b,
  const double delta_t,
  const Eigen::MatrixXd & fext,
  const Eigen::VectorXd & r,
  const Eigen::MatrixXd & M,
  const Eigen::MatrixXd & A,
  const Eigen::MatrixXd & C,
  const Eigen::LLT<Eigen::MatrixXd> & prefactorization,
  const Eigen::MatrixXd & Uprev,
  const Eigen::MatrixXd & Ucur,
  Eigen::MatrixXd & Unext)
{
  //////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  Unext = Ucur;  // Initial guess
  
  for(int iter = 0;iter < 50;iter++)
  {
    //std::cout << "1\n";
    // Local step
    Eigen::MatrixXd d = Eigen::MatrixXd::Zero(E.rows(), 3);
    for (int i = 0; i < E.rows(); i++) {
      d.row(i) = (Unext.row(E.row(i)[0]) - Unext.row(E.row(i)[1])).normalized() * r[i];
    }
    //std::cout << "2\n";

    // No clue whats going on, some math wizard magic
    Eigen::MatrixXd y = ((1 / (std::pow(delta_t, 2))) * M * ((2 * Ucur) - Uprev)) + fext;

    double w = 1e10;
    const Eigen::MatrixXd b = (k * A.transpose() * d + y) + 
      (w * C.transpose() * C * V); // Linear Penalty term

    //std::cout << "3\n";

    Unext = prefactorization.solve(b);
  }
  //////////////////////////////////////////////////////////////////////////////
}
