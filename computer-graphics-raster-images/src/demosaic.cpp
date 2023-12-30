#include "demosaic.h"
#include <iostream>

/**
 * Get the average color value from the 4 corner pixels to the target pixel.
*/
int average_corners(const std::vector<unsigned char> & bayer, int r, int c, int width, int height) {
  int sum = 0;
  int count = 0;

  // if statements ignore the corner if it would be out of bounds
  // if not out of bounds, it adds the bayer value to the sum and increments count
  if (r < height - 1 && c < width - 1) {
    sum += (int)bayer[((r + 1) * width) + (c + 1)];
    count++;
  }
  if (r > 0 && c < width - 1) {
    sum += (int)bayer[((r - 1) * width) + (c + 1)];
    count++;
  }
  if (r > 0 && c > 0) {
    sum += (int)bayer[((r - 1) * width) + (c - 1)];
    count++;
  }
  if (r < height - 1 && c > 0) {
    sum += (int)bayer[((r + 1) * width) + (c - 1)];
    count++;
  }

  return sum / count;
}

/**
 * Get the average color value from the 4 adjacent side pixels (top left bottom right) to the target pixel.
*/
int average_sides(const std::vector<unsigned char> & bayer, int r, int c, int width, int height) {
  int sum = 0;
  int count = 0;

  // Read two comments in "average_corners" for explanation
  if (r < height - 1) {
    sum += bayer[((r + 1) * width) + (c)];
    count++;
  }
  if (c < width - 1) {
    sum += bayer[((r) * width) + (c + 1)];
    count++;
  }
  if (r > 0) {
    sum += bayer[((r - 1) * width) + (c)];
    count++;
  }
  if (c > 0) {
    sum += bayer[((r) * width) + (c - 1)];
    count++;
  }
  
  return sum / count;
}

/**
 * Get the average color value from the 2 vertically adjacent side pixels (top bottom) to the target pixel.
*/
int average_vert(const std::vector<unsigned char> & bayer, int r, int c, int width, int height) {
  int sum = 0;
  int count = 0;

  // Read two comments in "average_corners" for explanation
  if (r < height - 1) {
    sum += bayer[((r + 1) * width) + (c)];
    count++;
  }
  if (r > 0) {
    sum += bayer[((r - 1) * width) + (c)];
    count++;
  }
  
  return sum / count;
}

/**
 * Get the average color value from the 2 horizontally adjacent side pixels (left right) to the target pixel.
*/
int average_hor(const std::vector<unsigned char> & bayer, int r, int c, int width, int height) {
  int sum = 0;
  int count = 0;

  // Read two comments in "average_corners" for explanation
  if (c < width - 1) {
    sum += bayer[((r) * width) + (c + 1)];
    count++;
  }
  if (c > 0) {
    sum += bayer[((r) * width) + (c - 1)];
    count++;
  }
  
  return sum / count;
}

void demosaic(
  const std::vector<unsigned char> & bayer,
  const int & width,
  const int & height,
  std::vector<unsigned char> & rgb)
{
  rgb.resize(width*height*3);
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here

  // GBRG pattern, except the 4 represents "green" in the case where green has red to the sides instead of blue
  int pattern[2][2] = {{1, 2}, {0, 4}};

  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      // Get channel
      int channel = pattern[r % 2][c % 2];
      int red_sides = 0;

      // If special green, reassign channel to 1 for green and set the flag for red sides
      if (channel == 4) {
        channel = 1;
        red_sides = 1;
      }
      
      // Record the channel into the rgb image directly from the mosaic
      rgb[(r * width * 3) + (c * 3) + channel] = bayer[(r * width) + c];

      // If red, then green will be computed as avg of sides and blue as avg of corners
      if (channel == 0) {
        rgb[(r * width * 3) + (c * 3) + 1] = average_sides(bayer, r, c, width, height);
        rgb[(r * width * 3) + (c * 3) + 2] = average_corners(bayer, r, c, width, height);
      // if green, compute red and blue from vertical and horizontal depending on the "red sides" case
      } else if (channel == 1) {
        if (red_sides) {
          rgb[(r * width * 3) + (c * 3) + 0] = average_hor(bayer, r, c, width, height);
          rgb[(r * width * 3) + (c * 3) + 2] = average_vert(bayer, r, c, width, height);
        } else {
          rgb[(r * width * 3) + (c * 3) + 2] = average_hor(bayer, r, c, width, height);
          rgb[(r * width * 3) + (c * 3) + 0] = average_vert(bayer, r, c, width, height);
        }
      // if blue, compute red from corners and green from sides
      } else if (channel == 2) {
        rgb[(r * width * 3) + (c * 3) + 0] = average_corners(bayer, r, c, width, height);
        rgb[(r * width * 3) + (c * 3) + 1] = average_sides(bayer, r, c, width, height);
      }

    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
