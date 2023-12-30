#include "AABBTree.h"
#include <iostream>

// See AABBTree.h for API
bool AABBTree::ray_intersect(
  const Ray& ray,
  const double min_t,
  const double max_t,
  double & t,
  std::shared_ptr<Object> & descendant) const 
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:

  // Debug block =======
  // bool d = false;
  // if (ray.origin[0] < 0 && ray.origin[1] < 0 && ray.origin[2] && std::abs(ray.origin[0] - -0.53361024313) < 1e-8) {
  //   d = true;
  // }
  // if (d) {std::cout << "{\n";}
  // =========

  // Check if the box of this AABB node is intersected
  if (ray_intersect_box(ray, box, min_t, max_t)) {
    double left_t;
    bool is_hit_left = false;
    double right_t;
    bool is_hit_right = false;
    std::shared_ptr<Object> obj_hit_left;
    std::shared_ptr<Object> obj_hit_right;

    // Intersect left tree
    if (left != nullptr) { 
      is_hit_left = left.get()->ray_intersect(ray, min_t, max_t, left_t, obj_hit_left);

      // If the left object is not an AABB Tree, it must be an actual object, so set the descendent
      std::shared_ptr<AABBTree> cast_attempt = std::dynamic_pointer_cast<AABBTree>(left);
      if (!cast_attempt) {
        obj_hit_left = left;
      }
    }

    // Intersect right tree
    if (right != nullptr) {
      is_hit_right = right.get()->ray_intersect(ray, min_t, max_t, right_t, obj_hit_right);

      // If the left object is not an AABB Tree, it must be an actual object, so set the descendent
      std::shared_ptr<AABBTree> cast_attempt = std::dynamic_pointer_cast<AABBTree>(right);
      if (!cast_attempt) {
        obj_hit_right = right;
      }
    }

    // No hits from either side, return false
    if ((!is_hit_left) && (!is_hit_right)) {
      // if (d) {std::cout << "No hits either side, return false\n}\n";}
      t = 0;
      return false;
    }

    // Compute closest t value and set correct descendent
    if (is_hit_left && (!is_hit_right)) {
      // if (d) {std::cout << "collision left only, return true\n}\n";} 
      t = left_t; descendant = obj_hit_left;
    }
    else if (is_hit_right && (!is_hit_left)) {
      // if (d) {std::cout << "collision right only, return true\n}\n";} 
      t = right_t; descendant = obj_hit_right;
    }
    else if (left_t < right_t) {
      // if (d) {std::cout << "collision both, left first, return true\n}\n";} 
      t = left_t; descendant = obj_hit_left;}
    else {
      // if (d) {std::cout << "collision both, right first, return true\n}\n";} 
      t = right_t; descendant = obj_hit_right;
    }
    
    return true;
  }
  // if (d) {std::cout << "Missed whole box, return false\n}\n";}
  t = 0;
  return false;
  ////////////////////////////////////////////////////////////////////////////
}

