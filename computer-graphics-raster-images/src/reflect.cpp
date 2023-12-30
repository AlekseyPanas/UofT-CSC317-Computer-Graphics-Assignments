#include "reflect.h"

void reflect(
  const std::vector<unsigned char> & input,
  const int width,
  const int height,
  const int num_channels,
  std::vector<unsigned char> & reflected)
{
  reflected.resize(width*height*num_channels);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      for (int j = 0; j < num_channels; j++) {
        // width - c reflects the pixel horizontally
        reflected[(r * width * num_channels) + (c * num_channels) + j] = 
            input[(r * width * num_channels) + ((width - c - 1) * num_channels) + j];
      }
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
