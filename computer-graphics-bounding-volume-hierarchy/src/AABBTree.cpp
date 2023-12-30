#include "AABBTree.h"
#include "insert_box_into_box.h"
#include <limits>
#include <iostream>

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object> > & objects,
  int a_depth): 
  depth(std::move(a_depth)), 
  num_leaves(objects.size())
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here

  //std::cout << std::string((int)(depth), ' ') << depth << " START\n";
  // Set initial object list bounding box values to infinity
  double mins[3] = {std::numeric_limits<double>::infinity(), 
  std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
  double maxs[3] = {-std::numeric_limits<double>::infinity(), 
  -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()};

  // For each object
  for (int i = 0; i < objects.size(); i++) {
    // For each component x,y,z
    for (int j = 0; j < 3; j++) {

      // If the object's component is outside the bounds, expand the bounds
      double obj_min = objects[i].get()->box.min_corner[j];
      double obj_max = objects[i].get()->box.max_corner[j];
      if (obj_min < mins[j]) {
        mins[j] = obj_min;
      }
      if (maxs[j] < obj_max) {
        maxs[j] = obj_max;
      }
        
    }
  }
  //std::cout << mins[0] << ", " << mins[1] << ", " << mins[2] << "          MAX: " << 
  //maxs[0] << ", " << maxs[1] << ", " << maxs[2] << "\n";
  

  // Compute which axis is longest to split against
  double longest_axis_length = -std::numeric_limits<double>::infinity();
  int longest_axis_i;
  for (int i = 0; i < 3; i++) {
    double dist = maxs[i] - mins[i];
    if (dist > longest_axis_length) {
      longest_axis_length = dist;
      longest_axis_i = i;
    }
  }  
  double longest_axis_midpoint = mins[longest_axis_i] + (longest_axis_length / 2);
  
  //std::cout << "Length: " << longest_axis_length << "        i: " << longest_axis_i << "        midpt: " <<
  //longest_axis_midpoint << "\n";

  // Set this tree's bounding box to the one computed earlier
  box.min_corner = Eigen::RowVector3d(mins[0], mins[1], mins[2]);
  box.max_corner = Eigen::RowVector3d(maxs[0], maxs[1], maxs[2]);

  std::vector<std::shared_ptr<Object>> left_tree;
  std::vector<std::shared_ptr<Object>> right_tree;
  std::vector<std::shared_ptr<Object>> undecided_tree;

  // Sort objects into their subtrees based on longest axis midpoint
  // There is a case where an object lingers on the midpoint, we want to avoid
  // an infinite loop where all objects sort to one side, so we mark it "undecided" and
  // after make sure it is placed to balance out the empty side
  for (int i = 0; i < objects.size(); i++) {
    double obj_center = objects[i].get()->box.center()[longest_axis_i];

    if (std::abs(obj_center - longest_axis_midpoint) < 1e-5) {
      undecided_tree.push_back(objects[i]);
    } else if (obj_center < longest_axis_midpoint) {
      left_tree.push_back(objects[i]);
    } else {
      right_tree.push_back(objects[i]);
    } 
  }
  // Sort undecided objects to the less populated tree
  for (int i = 0; i < undecided_tree.size(); i++) {
    if (left_tree.size() < right_tree.size()) {
      left_tree.push_back(undecided_tree[i]);
    } else {
      right_tree.push_back(undecided_tree[i]);
    }
  }

  //std::cout << left_tree.size() << ", " << right_tree.size() << "\n";

  // If single object, set that object as the subtree. If multiple, recursively build
  // If no objects, set null
  if (left_tree.size() == 1) {
    //std::cout << "Left Singular" << "\n\n";
    left = left_tree[0];
  } else if (left_tree.size() == 0) {
    //std::cout << "Left Null PTR" << "\n\n";
    left = nullptr;
  } else {
    //std::cout << "Left Multiple, Recursing" << "\n\n";
    left = std::make_shared<AABBTree>(left_tree, depth + 1);
  }

  if (right_tree.size() == 1) {
    //std::cout << "Right Singular" << "\n\n";
    right = right_tree[0];
  } else if (right_tree.size() == 0) {
    //std::cout << "Right Null PTR" << "\n\n";
    right = nullptr;
  } else {
    //std::cout << "Right Multiple, Recursing" << "\n\n";
    right = std::make_shared<AABBTree>(right_tree, depth + 1);
  }

  //std::cout << std::string(5, ' ') << depth << " FINISH\n";
  
  ////////////////////////////////////////////////////////////////////////////
}
