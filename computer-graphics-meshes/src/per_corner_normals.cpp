#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(F.rows()*3,3);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:
  double pi = 3.141592653589;  // PI :-)

  std::vector<std::vector<int>> vertex_incidents;  // Vertex index -> incident face indices
  vertex_triangle_adjacency(F, V.rows(), vertex_incidents); 

  // Loop through faces
  for (int f = 0; f < F.rows(); f++) {

    // Normal of this face
    Eigen::RowVector3d this_norm = triangle_area_normal(V.row(F.row(f)[0]), V.row(F.row(f)[1]), V.row(F.row(f)[2])).normalized();

    // For each of the 3 vertices
    for (int i = 0; i < 3; i++) {
      int vert = F.row(f)[i]; // This face's current vertex index

      // Tracks average
      double x = 0;
      double y = 0;
      double z = 0;
      double count = 0;      

      // Loop through all incident faces
      for (int j = 0; j < vertex_incidents[vert].size(); j++) {
        // Get the face index vector for the current incident face
        Eigen::RowVector3i face = F.row(vertex_incidents[vert][j]);

        // Norm of the current face
        Eigen::RowVector3d norm = triangle_area_normal(V.row(face[0]), V.row(face[1]), V.row(face[2]));

        // Angle between the incident face and the current outer loop face
        double dot = std::max(std::min(norm.normalized().dot(this_norm.normalized()), 1.0), -1.0);
        double ang_degrees = std::acos(dot) * (180 / pi);

        // If below thresh, add the normal vector to be averaged for this vertex
        if (ang_degrees <= corner_threshold) {
          x += norm[0];
          y += norm[1];
          z += norm[2];
          count++;
        };

      }
      // Average and normalize the normal
      N.row(f*3+i) = Eigen::RowVector3d(x / count, y / count, z / count).normalized();
    }
  }

  ////////////////////////////////////////////////////////////////////////////
}
