#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
  const std::string & filename,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & UV,
  const Eigen::MatrixXi & UF,
  const Eigen::MatrixXd & NV,
  const Eigen::MatrixXi & NF)
{
  assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here:

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

  for (int i = 0; i < V.rows(); i++) {
    ppmfile << "v " << V.row(i)[0] << " " <<  V.row(i)[1] << " " << V.row(i)[2] << '\n';
  }

  for (int i = 0; i < UV.rows(); i++) {
    ppmfile << "vt " << UV.row(i)[0] << " " <<  UV.row(i)[1] << '\n';
  }

  for (int i = 0; i < NV.rows(); i++) {
    ppmfile << "vn " << NV.row(i)[0] << " " <<  NV.row(i)[1] << " " << NV.row(i)[2] << '\n';
  }

  for (int i = 0; i < F.rows(); i++) {
    ppmfile << "f " << F.row(i)[0] << "/" << UF.row(i)[0] << "/" << NF.row(i)[0] << " "
                    << F.row(i)[1] << "/" << UF.row(i)[1] << "/" << NF.row(i)[1] << " " 
                    << F.row(i)[2] << "/" << UF.row(i)[2] << "/" << NF.row(i)[2] << " "
                    << F.row(i)[3] << "/" << UF.row(i)[3] << "/" << NF.row(i)[3] << " " << '\n';
  }

  ppmfile.close();

  return true;

  ////////////////////////////////////////////////////////////////////////////
}
