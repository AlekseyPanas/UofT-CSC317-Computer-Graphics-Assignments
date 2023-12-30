#include "rgb_to_gray.h"

void rgb_to_gray(
  const std::vector<unsigned char> & rgb,
  const int width,
  const int height,
  std::vector<unsigned char> & gray)
{
  gray.resize(height*width);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  for (int i = 0; i < width*height; i++) {
    // 0.2126 ∙ Red + 0.7152 ∙ Green + 0.0722 ∙ Blue
    gray[i] = (unsigned char)((double)rgb[i * 3 + 0] * 0.2126 +
                              (double)rgb[i * 3 + 1] * 0.7152 +
                              (double)rgb[i * 3 + 2] * 0.0722);
  }
  ////////////////////////////////////////////////////////////////////////////
}


