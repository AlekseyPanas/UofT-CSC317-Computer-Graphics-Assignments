#include "point_AABBTree_squared_distance.h"
#include <queue> // std::priority_queue
#include <limits>
#include <iostream>


bool point_AABBTree_squared_distance(
    const Eigen::RowVector3d & query,
    const std::shared_ptr<AABBTree> & root,
    const double min_sqrd,
    const double max_sqrd,
    double & sqrd,
    std::shared_ptr<Object> & descendant)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here

  // Create min-heap queue
  std::priority_queue<std::pair<double, std::shared_ptr<Object>>, 
          std::vector<std::pair<double, std::shared_ptr<Object>>>, 
          std::greater<std::pair<double, std::shared_ptr<Object>>>> queue;

  // Add root only if the distance is within bounds. (Not adding root causes immediate false return)
  double root_dist = point_box_squared_distance(query, root.get()->box);
  if (root_dist <= max_sqrd && root_dist >= min_sqrd) {
    queue.push(std::make_pair(root_dist, root));
  }

  // Track min distance, initially infinite
  double d = std::numeric_limits<double>::infinity();

  // Process queue if not empty
  while (queue.size() > 0) {
    // std::cout << "in loop\n";
    
    // Grab and pop top item's obj ptr and distance
    std::pair<double, std::shared_ptr<Object>> itm = queue.top();
    std::shared_ptr<Object> obj = itm.second;
    double dist = itm.first;
    queue.pop();

    // std::cout << dist << "\n";

    // Skip object if dist is larger than leaf min found
    if (dist < d) {
      // std::cout << "in if\n";
      // If leaf: update distance and set descendent
      std::shared_ptr<AABBTree> aabb = std::dynamic_pointer_cast<AABBTree>(obj);
      if (!aabb) {
        // std::cout << "LEAF\n";
        d = dist;
        descendant = obj;
      } 
      
      // Not leaf
      else {
        // Grab both children
        std::shared_ptr<Object> left = aabb.get()->left;
        std::shared_ptr<Object> right = aabb.get()->right;

        // Ignore null pointers
        if (left != nullptr) {
          std::shared_ptr<AABBTree> aabb_left = std::dynamic_pointer_cast<AABBTree>(left);

          // Set initially to satisfactory conditions
          double left_dist = min_sqrd;
          bool res = true;

          // If another subtree, compute box distance
          if (aabb_left) {
            left_dist = point_box_squared_distance(query, aabb_left.get()->box);
          } 
          // If leaf, compute pure distance
          else {
            res = left.get()->point_squared_distance(query, min_sqrd, max_sqrd, left_dist, descendant); // descendent irrelevant here
          }

          // Either the leaf must have had a valid distance according to bounds,
          // or the subtree's box dist must be within bounds. If so, add this object for investigation
          if (res && left_dist <= max_sqrd && left_dist >= min_sqrd) {
            queue.push(std::make_pair(left_dist, left));
          }
        }

        // Same for right child
        if (right != nullptr) {
          std::shared_ptr<AABBTree> aabb_right = std::dynamic_pointer_cast<AABBTree>(right);
          double right_dist = min_sqrd;
          bool res = true;

          if (aabb_right) {
            right_dist = point_box_squared_distance(query, aabb_right.get()->box);
          } else {
            res = right.get()->point_squared_distance(query, min_sqrd, max_sqrd, right_dist, descendant); // descendent irrelevant here
          }

          if (res && right_dist <= max_sqrd && right_dist >= min_sqrd) {
            queue.push(std::make_pair(right_dist, right));
          }
        }
      }
    }
  }

  // Set sqrd to the min distance found, if d was still infinite, return false since no min item was found
  // within set bounds
  sqrd = d;
  return !(d == std::numeric_limits<double>::infinity());
  ////////////////////////////////////////////////////////////////////////////
}
