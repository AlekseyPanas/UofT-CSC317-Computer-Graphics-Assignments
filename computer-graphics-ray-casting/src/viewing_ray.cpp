#include "viewing_ray.h"
#include <iostream>

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here

  // Compute camera matrix
  Eigen::MatrixXd camMatrix;
  camMatrix.resize(3, 3);
  camMatrix << camera.u, camera.v, camera.w;

  // Compute camera coordinates from pixel coordinates
  double u_i = camera.width * ((((double)j) + 0.5) / ((double)width)) - (camera.width * 0.5);
  double v_j = camera.height * ((((double)(height - i)) + 0.5) / ((double)height)) - (camera.height * 0.5);

  // Set origin and transform the direction vector into world coords
  ray.origin = camera.e;
  ray.direction = camMatrix * Eigen::Vector3d(u_i, v_j, -camera.d);

  ////////////////////////////////////////////////////////////////////////////
}

