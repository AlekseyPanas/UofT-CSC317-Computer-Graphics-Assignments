#include "TriangleSoup.h"
#include "Ray.h"
// Hint
#include "first_hit.h"

bool TriangleSoup::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:

  // Compute first hit across all the triangles
  int hit_id;
  bool is_hit = first_hit(ray, min_t, triangles, hit_id, t, n);

  if (!is_hit) {
    return false;
  }

  return true;
  ////////////////////////////////////////////////////////////////////////////
}



