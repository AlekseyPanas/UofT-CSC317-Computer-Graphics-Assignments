#include "over.h"
#include <iostream>

void over(
  const std::vector<unsigned char> & A,
  const std::vector<unsigned char> & B,
  const int & width,
  const int & height,
  std::vector<unsigned char> & C)
{
  C.resize(A.size());
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here

  for (int i = 0; i < width*height; i++) {
    // Get alphas, scale them
    double ab = ((double)B[i * 4 + 3]) / 255;
    double af = ((double)A[i * 4 + 3]) / 255;
    // Compute final alpha as the amount of light remaining after passing through both images
    double ao = af + ab * (1 - af);

    //std::cout << (double)A[i * 4 + 3] << "\n";
    //std::cout << "ab: " << ab << "    af: " << af << "    ao: " << ao << "\n";

    for (int j = 0; j < 3; j++) {
      // Compute colors as fractions of alphas, and scale to 100% by dividing by ao.
      C[i * 4 + j] = ( (af * A[i * 4 + j]) + ((1-af) * ab * B[i * 4 + j]) ) / ao;
      //std::cout << (int)C[i * 4 + j] << ", ";
    }

    // Set the alpha and scale it back
    C[i * 4 + 3] = ao * 255;

    //std::cout << "\n\n";
  }

  ////////////////////////////////////////////////////////////////////////////
}
