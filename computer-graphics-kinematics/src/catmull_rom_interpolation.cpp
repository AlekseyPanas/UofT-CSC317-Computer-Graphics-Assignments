#include "catmull_rom_interpolation.h"
#include <Eigen/Dense>
#include <iostream>

Eigen::Vector3d catmull_rom_interpolation(
  const std::vector<std::pair<double, Eigen::Vector3d> > & keyframes,
  double t)
{
  /////////////////////////////////////////////////////////////////////////////
  // Replace with your code

  // No keyframes, no movement
  if (keyframes.size() == 0) {
    return Eigen::Vector3d(0, 0, 0);
  }

  //std::cout << "CRI: START\n";

  // Indices into keyframes such that t0 <= t <= t1
  int t0_idx = 0;
  int t1_idx = keyframes.size() - 1;

  for (int i = 0; i < keyframes.size(); i++) {
    if (t == keyframes[i].first) {
      return keyframes[i].second;
    }

    if (keyframes[i].first <= t && keyframes[i].first > keyframes[t0_idx].first) {
      t0_idx = i;
    }

    if (keyframes[i].first >= t && keyframes[i].first < keyframes[t1_idx].first) {
      t1_idx = i;
    }

    //std::cout << keyframes[i].first << ", ";
  }
  //std::cout << "\n";
  //std::cout << "t0: " << keyframes[t0_idx].first << "      t: " << t << "    t1: " << keyframes[t1_idx].first << "\n";

  //std::cout << "CRI: END\n";

  // If time past animation end, return last keyframe
  if (t0_idx == t1_idx) {
    return keyframes[t0_idx].second;
  }

  //return keyframes[t0_idx].second; // For testing

  // Tangent vector computation via finite difference
  Eigen::Vector3d tangent_t0 = (keyframes[t1_idx].second - keyframes[t0_idx].second) / 
              (keyframes[t1_idx].first - keyframes[t0_idx].first);
  Eigen::Vector3d tangent_t1;
  if (t1_idx == keyframes.size() - 1) {  // If last keyframe, sets 0 tangent
    tangent_t1 = Eigen::Vector3d(0, 0, 0);
  } else {
    tangent_t1 = (keyframes[t1_idx + 1].second - keyframes[t1_idx].second) / 
              (keyframes[t1_idx + 1].first - keyframes[t1_idx].first);
  }

  double t_percent = (t - keyframes[t0_idx].first) / (keyframes[t1_idx].first - keyframes[t0_idx].first);

  //std::cout << "CRI: END\n";

  // Compute intermediary value via spline
  return ((2 * pow(t_percent, 3) - 3 * pow(t_percent, 2) + 1) * keyframes[t0_idx].second) + 
  ((pow(t_percent, 3) - 2 * pow(t_percent, 2) + t_percent) * tangent_t0) + 
  ((-2 * pow(t_percent, 3) + 3 * pow(t_percent, 2)) * keyframes[t1_idx].second) +
  ((pow(t_percent, 3) - pow(t_percent, 2)) * tangent_t1);
  /////////////////////////////////////////////////////////////////////////////
}
