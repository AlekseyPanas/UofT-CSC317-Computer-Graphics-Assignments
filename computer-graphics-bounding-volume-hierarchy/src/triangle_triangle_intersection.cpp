#include "triangle_triangle_intersection.h"
#include <iostream>

// For testing tree algorithm correctness
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h> 
// typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
// typedef Kernel::Point_3 Point;
// typedef Kernel::Triangle_3 Triangle;

int count_thresh(double thresh, double a, double b, double c, bool below) {
  if (below) {
    return ((int)(a < thresh)) + ((int)(b < thresh)) + ((int)(c < thresh));
  } else {
    return ((int)(a > thresh)) + ((int)(b > thresh)) + ((int)(c > thresh));
  }
  
}

bool triangle_triangle_intersection(
  const Eigen::RowVector3d & A0,
  const Eigen::RowVector3d & A1,
  const Eigen::RowVector3d & A2,
  const Eigen::RowVector3d & B0,
  const Eigen::RowVector3d & B1,
  const Eigen::RowVector3d & B2)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  // Implementing Deviller's method

  // std::cout << "HERE1\n";

  // Used to isolate my problem to the tree algorithm
  // Triangle t1(Point(A0[0], A0[1], A0[2]), 
  //             Point(A1[0], A1[1], A1[2]), 
  //             Point(A2[0], A2[1], A2[2]));

  // Triangle t2(Point(B0[0], B0[1], B0[2]), 
  //             Point(B1[0], B1[1], B1[2]), 
  //             Point(B2[0], B2[1], B2[2]));

  // return CGAL::do_intersect(t1, t2);

  // Checks which side each point from A is with respect to B's plane
  Eigen::Matrix<double, 4, 4> A0_Bplane;
  A0_Bplane << B0[0], B0[1], B0[2], 1,
              B1[0], B1[1], B1[2], 1,
              B2[0], B2[1], B2[2], 1,
              A0[0], A0[1], A0[2], 1;
  double A0_Bplane_det = A0_Bplane.determinant();
  Eigen::Matrix<double, 4, 4> A1_Bplane;
  A1_Bplane << B0[0], B0[1], B0[2], 1,
              B1[0], B1[1], B1[2], 1,
              B2[0], B2[1], B2[2], 1,
              A1[0], A1[1], A1[2], 1;
  double A1_Bplane_det = A1_Bplane.determinant();
  Eigen::Matrix<double, 4, 4> A2_Bplane;
  A2_Bplane << B0[0], B0[1], B0[2], 1,
              B1[0], B1[1], B1[2], 1,
              B2[0], B2[1], B2[2], 1,
              A2[0], A2[1], A2[2], 1;
  double A2_Bplane_det = A2_Bplane.determinant();

  // If all points of this triangle are on one side, there is no collision.
  // To save computation, we will consider the case where a triangle vertex is exactly
  // on the intersection line as a non-collision
  // This also eliminates the coplanar case
  int num_below_B = count_thresh(0, A0_Bplane_det, A1_Bplane_det, A2_Bplane_det, true);
  if (num_below_B == 3 || num_below_B == 0 || A0_Bplane_det == 0 || A1_Bplane_det == 0 || A2_Bplane_det == 0) {
    return false;
  }

  Eigen::Matrix<double, 4, 4> B0_Aplane;
  B0_Aplane << A0[0], A0[1], A0[2], 1,
              A1[0], A1[1], A1[2], 1,
              A2[0], A2[1], A2[2], 1,
              B0[0], B0[1], B0[2], 1;
  double B0_Aplane_det = B0_Aplane.determinant();
  Eigen::Matrix<double, 4, 4> B1_Aplane;
  B1_Aplane << A0[0], A0[1], A0[2], 1,
              A1[0], A1[1], A1[2], 1,
              A2[0], A2[1], A2[2], 1,
              B1[0], B1[1], B1[2], 1;
  double B1_Aplane_det = B1_Aplane.determinant();
  Eigen::Matrix<double, 4, 4> B2_Aplane;
  B2_Aplane << A0[0], A0[1], A0[2], 1,
              A1[0], A1[1], A1[2], 1,
              A2[0], A2[1], A2[2], 1,
              B2[0], B2[1], B2[2], 1;
  double B2_Aplane_det = B2_Aplane.determinant();

  // Confirms other triangle is 1 point 2 point split across other triangle's plane
  int num_below_A = count_thresh(0, B0_Aplane_det, B1_Aplane_det, B2_Aplane_det, true);
  if (num_below_A == 3 || num_below_A == 0 || B0_Aplane_det == 0 || B1_Aplane_det == 0 || B2_Aplane_det == 0) {
    return false;
  }

  // Tricky bit. We now orient the points in such a way such that p1/p2 for each triangle
  // represents the single isolated point in the "positive" side of the other triangle's plane
  // (Read Deviller's paper, took me a good few minutes to understand)
  Eigen::RowVector3d pa;
  Eigen::RowVector3d qa;
  Eigen::RowVector3d ra;
  Eigen::RowVector3d pb;
  Eigen::RowVector3d qb;
  Eigen::RowVector3d rb;

  if (num_below_B == 1) {
    if (A0_Bplane_det < 0) {
      pa = A0;
      qa = A1;
      ra = A2;
    } else if (A1_Bplane_det < 0) {
      pa = A1;
      qa = A0;
      ra = A2;
    } else {
      pa = A2;
      qa = A1;
      ra = A0;
    }
  } else {
    if (A0_Bplane_det > 0) {
      pa = A0;
      qa = A1;
      ra = A2;
    } else if (A1_Bplane_det > 0) {
      pa = A1;
      qa = A0;
      ra = A2;
    } else {
      pa = A2;
      qa = A1;
      ra = A0;
    }
  }

  if (num_below_A == 1) {
    if (B0_Aplane_det < 0) {
      pb = B0;
      qb = B1;
      rb = B2;
    } else if (B1_Aplane_det < 0) {
      pb = B1;
      qb = B0;
      rb = B2;
    } else {
      pb = B2;
      qb = B1;
      rb = B0;
    }
  } else {
    if (B0_Aplane_det > 0) {
      pb = B0;
      qb = B1;
      rb = B2;
    } else if (B1_Aplane_det > 0) {
      pb = B1;
      qb = B0;
      rb = B2;
    } else {
      pb = B2;
      qb = B1;
      rb = B0;
    }
  }

  // Ensures that pa are on the positive side by swapping qb rb
  Eigen::Matrix<double, 4, 4> pa_Bplane;
  pa_Bplane << pb[0], pb[1], pb[2], 1,
              qb[0], qb[1], qb[2], 1,
              rb[0], rb[1], rb[2], 1,
              pa[0], pa[1], pa[2], 1;
  double pa_Bplane_det = pa_Bplane.determinant();

  // If pa is found to be on the "negative" size with respect to the orientation of b's points, swap b's points
  if (pa_Bplane_det < 0) {
    Eigen::RowVector3d temp = qb;
    qb = rb;
    rb = temp;
  }

  // Same for pb
  Eigen::Matrix<double, 4, 4> pb_Aplane;
  pb_Aplane << pa[0], pa[1], pa[2], 1,
              qa[0], qa[1], qa[2], 1,
              ra[0], ra[1], ra[2], 1,
              pb[0], pb[1], pb[2], 1;
  double pb_Aplane_det = pb_Aplane.determinant();

  if (pb_Aplane_det < 0) {
    Eigen::RowVector3d temp = qa;
    qa = ra;
    ra = temp;
  }

  // We now know that the four pa/pb incident edges intersect the plane intersection line, so we 
  // use the determinant trick presented in the paper to see if they intersect
  Eigen::Matrix<double, 4, 4> interval1;
  interval1 << pa[0], pa[1], pa[2], 1,
              qa[0], qa[1], qa[2], 1,
              pb[0], pb[1], pb[2], 1,
              qb[0], qb[1], qb[2], 1;
  double interval1_det = interval1.determinant();

  Eigen::Matrix<double, 4, 4> interval2;
  interval2 << pa[0], pa[1], pa[2], 1,
              ra[0], ra[1], ra[2], 1,
              rb[0], rb[1], rb[2], 1,
              pb[0], pb[1], pb[2], 1;
  double interval2_det = interval2.determinant();

  return interval1_det <= 0 && interval2_det <= 0;
  //return interval1_det <= 0 && interval2_det >= 0;  Intentionally broken for testing
  ////////////////////////////////////////////////////////////////////////////
}
