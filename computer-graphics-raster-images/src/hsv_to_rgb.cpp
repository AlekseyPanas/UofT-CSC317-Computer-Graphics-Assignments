#include "hsv_to_rgb.h"
#include <algorithm>
#include <cmath>

/**
 * Returns module between doubles and always ensures positive output
*/
double dmod2(double a, int b) {
  while (! (a < b && a >= 0)) {
    if (a >= b) {
      a -= b;
    } else {
      a += b;
    }
  }
  return a;
}

void hsv_to_rgb(
  const double h,
  const double s,
  const double v,
  double & r,
  double & g,
  double & b)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  double chroma = v * s;
  double h1 = h / 60; 
  double X = chroma * (1 - std::abs(dmod2(h1, 2) - 1));
  double m = v - chroma;

  // undefined case
  if (h == -60) {
    r = m * 255;
    g = m * 255;
    b = m * 255;
  } else {
    double triplets[6][3] = {{chroma, X, 0}, {X, chroma, 0}, {0, chroma, X}, {0, X, chroma}, {X, 0, chroma}, {chroma, 0, X}};
    double *chosen = triplets[(int)std::floor(h1)];

    r = (chosen[0] + m) * 255;
    g = (chosen[1] + m) * 255;
    b = (chosen[2] + m) * 255;
  }
  ////////////////////////////////////////////////////////////////////////////
}
