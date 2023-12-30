#include "sphere.h"
#include <iostream>

void sphere(
  const int num_faces_u,
  const int num_faces_v,
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  int num_faces_v_mod = num_faces_v / 2;
  
  double pi = 3.141592653589;
  double u_inc = (2 * pi) / num_faces_u;
  double v_inc = (pi) / num_faces_v_mod;

  int num_faces_u_mod = num_faces_u + 1;
  num_faces_v_mod++;

  V.resize(1, 3);
  UV.resize(1, 2);
  NV.resize(1, 3);
  F.resize(1, 4);
  UF.resize(1, 4);
  NF.resize(1, 4);

  bool first_face_added = false;

  for (int u = 0; u < num_faces_u_mod; u++) {
    for (int v = 0; v < num_faces_v_mod; v++) {
      double ang_u = u_inc * u;
      double ang_v = v_inc * v - (pi / 2);

      double x = std::cos(ang_v) * std::cos(ang_u);
      double y = std::cos(ang_v) * std::sin(ang_u);
      double z = std::sin(ang_v);

      if (! (u == 0 && v == 0)) {
        V.conservativeResize(V.rows() + 1, Eigen::NoChange);
        UV.conservativeResize(UV.rows() + 1, Eigen::NoChange);
        NV.conservativeResize(NV.rows() + 1, Eigen::NoChange);
      } 

      V.row((u * num_faces_v_mod) + v) = Eigen::RowVector3d(x, y, z);
      NV.row((u * num_faces_v_mod) + v) = Eigen::RowVector3d(x, y, z).normalized();
      UV.row((u * num_faces_v_mod) + v) = Eigen::RowVector2d(ang_u / (2 * pi), (ang_v + (pi/2)) / pi);

      if (u > 0 && v > 0) {
        if (first_face_added) {
          F.conservativeResize(F.rows() + 1, Eigen::NoChange);
          UF.conservativeResize(UF.rows() + 1, Eigen::NoChange);
          NF.conservativeResize(NF.rows() + 1, Eigen::NoChange);
        }
        if (!first_face_added) {first_face_added = true;}

        int i_1 = ((u-1) * num_faces_v_mod + (v-1)) + 1;
        int i_2 = ((u) * num_faces_v_mod + (v-1)) + 1;
        int i_3 = ((u) * num_faces_v_mod + (v)) + 1;
        int i_4 = ((u-1) * num_faces_v_mod + (v)) + 1;

        F.row(F.rows() - 1) = Eigen::RowVector4i(i_1, i_2, i_3, i_4);
        UF.row(UF.rows() - 1) = Eigen::RowVector4i(i_1, i_2, i_3, i_4);
        NF.row(NF.rows() - 1) = Eigen::RowVector4i(i_1, i_2, i_3, i_4);
      }
    }
  }

  ////////////////////////////////////////////////////////////////////////////
}
