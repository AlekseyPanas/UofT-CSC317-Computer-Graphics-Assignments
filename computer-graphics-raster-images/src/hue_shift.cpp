#include "hue_shift.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"
#include <cmath>
#include <algorithm>

void hue_shift(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double shift,
  std::vector<unsigned char> & shifted)
{
  shifted.resize(rgb.size());
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  //for (int i = 0; i < 1; i++) {
  for (int i = 0; i < width*height; i++) {
    double h;
    double s;
    double v;

    rgb_to_hsv((double)rgb[i * 3 + 0], (double)rgb[i * 3 + 1], (double)rgb[i * 3 + 2], h, s, v);
    h = std::fmod(h + shift, 360);

    double new_r;
    double new_g;
    double new_b;
    
    hsv_to_rgb(h, s, v, new_r, new_g, new_b);

    shifted[i * 3 + 0] = (int)new_r;
    shifted[i * 3 + 1] = (int)new_g;
    shifted[i * 3 + 2] = (int)new_b;
  }
  ////////////////////////////////////////////////////////////////////////////
}
