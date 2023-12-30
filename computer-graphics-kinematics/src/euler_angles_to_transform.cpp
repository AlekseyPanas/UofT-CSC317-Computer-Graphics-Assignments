#include "euler_angles_to_transform.h"

Eigen::Affine3d euler_angles_to_transform(
  const Eigen::Vector3d & xzx)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code
  Eigen::Affine3d A;

  Eigen::MatrixXd Rx1;
  Eigen::MatrixXd Rz;
  Eigen::MatrixXd Rx2;
  Rz.resize(4, 4);
  Rx1.resize(4, 4);
  Rx2.resize(4, 4);

  double pi = 3.141592653589;

  double x1_ang = (xzx[0] / 180) * pi;
  double z_ang = (xzx[1] / 180) * pi;
  double x2_ang = (xzx[2] / 180) * pi;

  Rx1 << 1, 0, 0, 0,
        0, std::cos(x1_ang), -std::sin(x1_ang), 0,
        0, std::sin(x1_ang), std::cos(x1_ang), 0,
        0, 0, 0, 1;

  Rz << std::cos(z_ang), -std::sin(z_ang), 0, 0,
        std::sin(z_ang), std::cos(z_ang), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

  Rx2 << 1, 0, 0, 0, 
        0, std::cos(x2_ang), -std::sin(x2_ang), 0,
        0, std::sin(x2_ang), std::cos(x2_ang), 0,
        0, 0, 0, 1;

  A.matrix() = Rx2 * Rz * Rx1;

  return A;
  /////////////////////////////////////////////////////////////////////////////
}
