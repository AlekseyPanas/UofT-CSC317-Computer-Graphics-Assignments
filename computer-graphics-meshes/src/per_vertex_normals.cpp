#include "per_vertex_normals.h"
#include "triangle_area_normal.h"

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(V.rows(),3);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:

  // For each vertex, stores face normals of every face that vertex belongs to
  std::vector<Eigen::RowVector3d> cache[V.rows()];

  for (int f = 0; f < F.rows(); f++) {
    // Add normal to cache for every vertex of this face
    Eigen::RowVector3d norm = triangle_area_normal(V.row(F.row(f)[0]), V.row(F.row(f)[1]), V.row(F.row(f)[2]));
    cache[F.row(f)[0]].push_back(norm);
    cache[F.row(f)[1]].push_back(norm);
    cache[F.row(f)[2]].push_back(norm);
  }

  for (int i = 0; i < V.rows(); i++) {
    float x = 0;
    float y = 0;
    float z = 0;
    float total = 0;

    for (int j = 0; j < cache[i].size(); j++) {
      x += cache[i][j][0];
      y += cache[i][j][1];
      z += cache[i][j][2];
      total++;
    }
    x = x / total;
    y = y / total;
    z = z / total;

    N.row(i) = Eigen::RowVector3d(x, y, z).normalized();
  }

  ////////////////////////////////////////////////////////////////////////////
}
