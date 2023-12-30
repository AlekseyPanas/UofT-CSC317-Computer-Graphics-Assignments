#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Geometry>
#include <iostream>

bool Triangle::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  
  // Get two triangle direction vectors using corners[0] as origin
  Eigen::Vector3d t1 = std::get<1>(corners) - std::get<0>(corners);
  Eigen::Vector3d t2 = std::get<2>(corners) - std::get<0>(corners);

  // shift ray origin by corners[0] triangle origin
  Eigen::Vector3d shifted_origin = ray.origin - std::get<0>(corners);

  // Compute the triangle matrix as in the triangle collision formula
  Eigen::MatrixXd triangle_matrix;
  triangle_matrix.resize(3, 3);
  triangle_matrix << t1, t2, -ray.direction;

  // use determinant to determine if this matrix is invertible (within 0 by 1e-9 to account for float inaccuracies)
  double det = triangle_matrix.determinant();
  if (det - round(det) <= 1e-9 && round(det) == 0) {
    return false;
  }

  // Get inverse matrix and compute the solutions vector <alpha, beta, t>
  Eigen::MatrixXd inversed = triangle_matrix.inverse();
  Eigen::Vector3d solutions = inversed * shifted_origin;

  // extract solutions
  double alpha = solutions[0];
  double beta = solutions[1];
  t = solutions[2];

  // Normal to two triangle direction vectors is the normal to the triangle
  n = t1.cross(t2).normalized();

  // Check to ensure alpha and beta conditions are satisfied, and that t is at least min_t
  if (alpha >= 0 && beta >= 0 && alpha + beta <= 1 && t >= min_t) {
    return true;
  }

  return false;
  ////////////////////////////////////////////////////////////////////////////
}


