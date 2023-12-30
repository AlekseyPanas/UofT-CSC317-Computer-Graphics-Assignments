#include "rgb_to_hsv.h"
#include <algorithm>
#include <cmath>
#include <iostream>

/**
 * Returns module between doubles and always ensures positive output
*/
double dmod1(double a, int b) {
  while (! (a < b && a >= 0)) {
    if (a >= b) {
      a -= b;
    } else {
      a += b;
    }
  }
  return a;
}

void rgb_to_hsv(
  const double r,
  const double g,
  const double b,
  double & h,
  double & s,
  double & v)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  double r1 = r / 255;
  double g1 = g / 255;
  double b1 = b / 255;

  double max = std::max(std::max(r1, g1), b1);
  double min = std::min(std::min(r1, g1), b1);
  double diff = max - min;

  double h1;
  if (diff == 0) {
    h1 = -1;
  } else if (max == r1) {
    h1 = dmod1(((g1 - b1) / diff), 6);
  } else if (max == g1) {
    h1 = ((b1 - r1) / diff) + 2;
  } else if (max == b1) {
    h1 = ((r1 - g1) / diff) + 4;
  } else {
    std::cout << "Big problem";
  }

  h = 60 * h1;
  s = 0;
  if (max != 0) {
    s = diff / max;
  }
  v = max;
  ////////////////////////////////////////////////////////////////////////////
}
