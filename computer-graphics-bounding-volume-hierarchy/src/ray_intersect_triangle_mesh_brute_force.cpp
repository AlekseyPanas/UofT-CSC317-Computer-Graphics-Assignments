#include "ray_intersect_triangle_mesh_brute_force.h"
#include "ray_intersect_triangle.h"
#include <iostream>

bool ray_intersect_triangle_mesh_brute_force(
  const Ray & ray,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double min_t,
  const double max_t,
  double & hit_t,
  int & hit_f)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:

  bool is_hit = false;
  hit_t = 0;
  hit_f = 0;

  for (int i = 0; i < F.rows(); i++) {
    double t;
    bool res = ray_intersect_triangle(ray, V.row(F.row(i)[0]), V.row(F.row(i)[1]), V.row(F.row(i)[2]), min_t, max_t, t);

    // First triangle hit, set the t and set true
    if ((!is_hit) && res) {
      is_hit = true;
      hit_t = t;
      hit_f = i;
    } 
    // Subsequent hits compare with t
    else if (res && t < hit_t) {
      hit_t = t;
      hit_f = i;
    }  
  }

  return is_hit;
  ////////////////////////////////////////////////////////////////////////////
}
