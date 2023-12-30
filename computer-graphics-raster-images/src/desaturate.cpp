#include "desaturate.h"
#include "hsv_to_rgb.h"
#include "rgb_to_hsv.h"

void desaturate(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  const double factor,
  std::vector<unsigned char> & desaturated)
{
  desaturated.resize(rgb.size());
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  //for (int i = (107 * width) + 325; i < (107 * width) + 326; i++) {
  for (int i = 0; i < width*height; i++) {
    double h;
    double s;
    double v;

    rgb_to_hsv((double)rgb[i * 3 + 0], (double)rgb[i * 3 + 1], (double)rgb[i * 3 + 2], h, s, v);

    s -= (s * factor);

    double new_r;
    double new_g;
    double new_b;
    
    hsv_to_rgb(h, s, v, new_r, new_g, new_b);

    desaturated[i * 3 + 0] = (int)new_r;
    desaturated[i * 3 + 1] = (int)new_g;
    desaturated[i * 3 + 2] = (int)new_b;
  }
  ////////////////////////////////////////////////////////////////////////////
}
