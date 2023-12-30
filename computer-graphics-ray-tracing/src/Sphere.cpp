#include "Sphere.h"
#include "Ray.h"

bool Sphere::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  
  // Shift the ray origin to account for shifted sphere center
  Eigen::Vector3d shifted_origin = ray.origin - center;

  // Compute quadratic coefficients according to collision formula
  double a = ray.direction.dot(ray.direction);
  double b = 2 * (shifted_origin.dot(ray.direction));
  double c = shifted_origin.dot(shifted_origin) - (radius * radius);

  // Find discriminant to determine number of solutions
  double discriminant = (b * b) - (4 * a * c);

  // negative indicates no solutions
  if (discriminant < 0) {
    return false;
  } else {
    // Solve for both solutions using quadratic formula
    double sln1 = (-b + sqrt(discriminant)) / (2 * a);
    double sln2 = (-b - sqrt(discriminant)) / (2 * a);

    // Return correct solution based on min_t and min solution
    if (sln1 < min_t && sln2 < min_t) {
      return false;
    } else if (sln1 < min_t) {
      t = sln2;
    } else if (sln2 < min_t) {
      t = sln1;
    } else {
      t = std::min(sln1, sln2);
    }

    // Compute normal
    n = (ray.origin + (t * ray.direction) - center).normalized(); 

    return true;
  }
  ////////////////////////////////////////////////////////////////////////////
}

