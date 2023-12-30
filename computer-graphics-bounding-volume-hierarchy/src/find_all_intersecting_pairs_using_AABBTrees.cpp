#include "find_all_intersecting_pairs_using_AABBTrees.h"
#include "box_box_intersect.h"
// Hint: use a list as a queue
#include <list> 
#include <iostream>

void find_all_intersecting_pairs_using_AABBTrees(
  const std::shared_ptr<AABBTree> & rootA,
  const std::shared_ptr<AABBTree> & rootB,
  std::vector<std::pair<std::shared_ptr<Object>,std::shared_ptr<Object> > > & 
    leaf_pairs)
{
  ////////////////////////////////////////////////////////////////////////////
  // Add your code here

  leaf_pairs.clear();
  std::list<std::pair<std::shared_ptr<Object>,std::shared_ptr<Object>>> queue;

  if (box_box_intersect(rootA.get()->box, rootB.get()->box)) {
    queue.push_front(std::make_pair(rootA, rootB));
  }

  while (queue.size() > 0) {
    std::pair<std::shared_ptr<Object>,std::shared_ptr<Object>> pair = queue.back();
    queue.pop_back();
    std::shared_ptr<Object> F = pair.first;
    std::shared_ptr<Object> S = pair.second;
    //std::cout << (F == nullptr) << "     " << (S == nullptr) << "\n" << std::endl;
    std::shared_ptr<AABBTree> F_AABB = std::dynamic_pointer_cast<AABBTree>(F);
    std::shared_ptr<AABBTree> S_AABB = std::dynamic_pointer_cast<AABBTree>(S);

    // BOTH LEAVES
    if ((!F_AABB) && (!S_AABB)) {
      leaf_pairs.push_back(std::make_pair(F, S));
    } 
    
    // F leaf, S Tree
    else if ((!F_AABB) && S_AABB) {
      if (S_AABB.get()->left != nullptr && box_box_intersect(F.get()->box, S_AABB.get()->left.get()->box)) {
        queue.push_front(std::make_pair(F, S_AABB.get()->left));
      } 
      if (S_AABB.get()->right != nullptr && box_box_intersect(F.get()->box, S_AABB.get()->right.get()->box)) {
        queue.push_front(std::make_pair(F, S_AABB.get()->right));
      }
    }

    // S Leaf, F Tree
    else if ((!S_AABB) && F_AABB) {
      if (F_AABB.get()->left != nullptr && box_box_intersect(S.get()->box, F_AABB.get()->left.get()->box)) {
        queue.push_front(std::make_pair(F_AABB.get()->left, S));
      } 
      if (F_AABB.get()->right != nullptr && box_box_intersect(S.get()->box, F_AABB.get()->right.get()->box)) {
        queue.push_front(std::make_pair(F_AABB.get()->right, S));
      }
    }

    // Both trees
    else {
      if (F_AABB.get()->left != nullptr && S_AABB.get()->left != nullptr && 
          box_box_intersect(F_AABB.get()->left.get()->box, S_AABB.get()->left.get()->box)) {
        queue.push_front(std::make_pair(F_AABB.get()->left, S_AABB.get()->left));
      }

      if (F_AABB.get()->left != nullptr && S_AABB.get()->right != nullptr && 
          box_box_intersect(F_AABB.get()->left.get()->box, S_AABB.get()->right.get()->box)) {
        queue.push_front(std::make_pair(F_AABB.get()->left, S_AABB.get()->right));
      }

      if (F_AABB.get()->right != nullptr && S_AABB.get()->left != nullptr && 
          box_box_intersect(F_AABB.get()->right.get()->box, S_AABB.get()->left.get()->box)) {
        queue.push_front(std::make_pair(F_AABB.get()->right, S_AABB.get()->left));
      }

      if (F_AABB.get()->right != nullptr && S_AABB.get()->right != nullptr && 
          box_box_intersect(F_AABB.get()->right.get()->box, S_AABB.get()->right.get()->box)) {
        queue.push_front(std::make_pair(F_AABB.get()->right, S_AABB.get()->right));
      }
    } 
  }

  // std::cout<< leaf_pairs.size() << "\n";
  // std::cout << "This function returns" << "\n";
  ////////////////////////////////////////////////////////////////////////////
}
