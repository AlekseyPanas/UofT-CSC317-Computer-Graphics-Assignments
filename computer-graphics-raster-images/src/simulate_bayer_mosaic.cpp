#include "simulate_bayer_mosaic.h"
#include <iostream>

void simulate_bayer_mosaic(
  const std::vector<unsigned char> & rgb,
  const int & width,
  const int & height,
  std::vector<unsigned char> & bayer)
{
  bayer.resize(width*height);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here

  // GBRG pattern in a 2D array
  int pattern[2][2] = {{1, 2}, {0, 1}};

  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {

      // By using modulo, we can tell which part of the pattern we are currently on
      int channel = pattern[r % 2][c % 2];
      
      // Set the bayer value at the current index to the rgb value of the current channel
      bayer[r * width + c] = rgb[(r * width * 3) + (c * 3) + channel];
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
