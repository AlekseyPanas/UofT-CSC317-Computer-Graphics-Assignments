#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  assert(
    (num_channels == 3 || num_channels ==1 ) &&
    ".ppm only supports RGB or grayscale images");
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:

  // Create stream, open file, error check
  std::ofstream ppmfile;
  try {
    ppmfile.open(filename); 
  } catch (...) {
    return false;
  }
  if (ppmfile.fail()) {
    return false;
  }

  // Set corresponding magic number
  if (num_channels == 1) {
    ppmfile << "P2 \n";
  } else {
    ppmfile << "P3 \n";
  }

  // Set width, height, and max pixel value (255)
  ppmfile << std::to_string(width) << " " << std::to_string(height) << " \n" << "255 \n";

  // record each channel (could have been done in one loop, but 
  // done this way to create a newline in the file after each rgb sequence)
  for (int i = 0; i < width * height; i++) {
    for (int j = 0; j < num_channels; j++) {
      ppmfile << std::to_string(data[i * num_channels + j]) << " ";
    }
    ppmfile << " \n";
  }

  return true;
  ////////////////////////////////////////////////////////////////////////////
}
