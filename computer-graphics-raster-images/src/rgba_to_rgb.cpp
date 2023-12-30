#include "rgba_to_rgb.h"

void rgba_to_rgb(
  const std::vector<unsigned char> & rgba,
  const int & width,
  const int & height,
  std::vector<unsigned char> & rgb)
{
  int size = height*width*3;
  rgb.resize(size);

  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  for (int i = 0; i < height*width; i++) {
    rgb[(i * 3) + 0] = rgba[(i * 4) + 0];
    rgb[(i * 3) + 1] = rgba[(i * 4) + 1];
    rgb[(i * 3) + 2] = rgba[(i * 4) + 2];
  ////////////////////////////////////////////////////////////////////////////
  }
}
