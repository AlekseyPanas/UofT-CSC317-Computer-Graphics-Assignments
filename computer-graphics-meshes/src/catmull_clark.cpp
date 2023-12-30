#include "catmull_clark.h"
#include <unordered_map>
#include <utility>
#include <functional>
#include <set>
#include <iostream>

void print_double_int_vec(std::vector<std::vector<int>> & vec) {
  std::cout << "\n";
  for(int i = 0; i < vec.size();i++) {for(int j = 0; j < vec[i].size();j++){
    std::cout << vec[i][j] << ", ";
  } std::cout << "\n";}
}

void print_int_vec(std::vector<int> & vec) {
  for(int j = 0; j < vec.size();j++){
    std::cout << vec[j] << ", ";
  }
  std::cout << "\n";
}

void print_vec_vec(std::vector<Eigen::RowVector3d> & vec) {
  for(int j = 0; j < vec.size();j++){
    std::cout << vec[j] << "\n";
  }
  std::cout << "\n";
}

/**
 * Return edge avg index of edge average between the two given faces, or -1 if not cached
*/
int in_map(int f1, int f2, std::unordered_map<std::string, int> & m) {
  std::string forward = std::to_string(f1) + ":" + std::to_string(f2);
  std::string backward = std::to_string(f2) + ":" + std::to_string(f1);

  if (m.find(forward) != m.end()) {
    return m[forward];
  } else if (m.find(backward) != m.end()) {
    return m[backward];
  }
  return -1;
}

void catmull_clark(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const int num_iters,
  Eigen::MatrixXd & SV,
  Eigen::MatrixXi & SF)
{
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  Eigen::MatrixXd Vcur;
  Eigen::MatrixXi Fcur;

  if (num_iters > 1) {
    catmull_clark(V, F, num_iters - 1, Vcur, Fcur);
  } else if (num_iters == 1) {
    Vcur = V;
    Fcur = F;
  } else {
    std::cout << "Mmmm you bad bad boy (or girl), putting negative or 0 iteration count... Who knows what will happen now\n";
  }



  SV.resize(Vcur.rows(), 3);
  SF.resize(Fcur.rows() * 4, 4);







  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






  // Compute vertex-face adjacency
  std::vector<std::vector<int>> vidx_to_list_fidx;
  vidx_to_list_fidx.resize(Vcur.rows());
  for (int f = 0; f < Fcur.rows(); f++) {
    vidx_to_list_fidx[Fcur.row(f)[0]].push_back(f);
    vidx_to_list_fidx[Fcur.row(f)[1]].push_back(f);
    vidx_to_list_fidx[Fcur.row(f)[2]].push_back(f);
    vidx_to_list_fidx[Fcur.row(f)[3]].push_back(f);
  }

  // print_double_int_vec(vidx_to_list_fidx);







  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







  // list of new edge average vertices
  std::vector<Eigen::RowVector3d> eidx_to_eavg(0);

  // Two face indices mapping to the edge between them
  std::unordered_map<std::string, int> f2idx_to_eidx;

  // maps face index to counter-clockwise list of indices into eidx_to_eavg for the face
  std::vector<std::vector<int>> fidx_to_ccw_eidx;
  fidx_to_ccw_eidx.resize(Fcur.rows());

  // maps face index to face average point
  Eigen::MatrixXd fidx_to_favg;
  fidx_to_favg.resize(Fcur.rows(), 3);






  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










  // Generate face average vertices
  for (int f = 0; f < Fcur.rows(); f++) {
    // Compute face average and add to map
    Eigen::RowVector3d favg(0.0, 0.0, 0.0);
    for (int v = 0; v < 4; v++) {
      favg = favg + Vcur.row(Fcur.row(f)[v]);
    }
    favg = favg / 4.0;
    fidx_to_favg.row(f) = favg;
  }

  // std::cout << "\n\n fidx_to_favg:\n" << fidx_to_favg << "\n";







  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








  // std::cout << "\n\n\n EDGE LOOP:\n\n";

  // Generate edge average vertices. Loop through faces
  for (int f = 0; f < Fcur.rows(); f++) {

    // std::cout << "\tFace: " << f << "\n";

    // Loop through edges of face
    for (int e = 0; e < 4; e++) {
      // Sets the indices of this edge's endpoints in f
      int e1_idx = e;
      int e2_idx = e == 3 ? 0 : e+1;
      // std::cout << "\t\tEdge indices into face: " << e1_idx << ", " << e2_idx << "\n";
      
      // Face adjacent to the edge != to the current face being looped
      int edge_face;

      // Gets all faces adjacent to each edge endpoint
      std::vector<int> e1_faces = vidx_to_list_fidx[Fcur.row(f)[e1_idx]];
      std::vector<int> e2_faces = vidx_to_list_fidx[Fcur.row(f)[e2_idx]];
      
      // std::cout << "\t\t endpt1, endpt2, adjacent faces:\n\t\t";
      // print_int_vec(e1_faces);
      // std::cout << "\t\t";
      // print_int_vec(e2_faces);

      
      for (int e1_f = 0; e1_f < e1_faces.size(); e1_f++) { // Adjacent faces of e1 edge endpt
        for (int e2_f = 0; e2_f < e2_faces.size(); e2_f++) { // Adjacent faces of e1 edge endpt
          // If the endpts share an adjacent face != f, its the second face of the edge
          if (e1_faces[e1_f] == e2_faces[e2_f] && e1_faces[e1_f] != f) {
            edge_face = e1_faces[e1_f]; // There should only be one!
            // std::cout << "\t\tCounterpart Face: " << edge_face << "\n";
          }
        }
      }

      // Check if in map and get the eidx_to_eavg index
      int eidx = in_map(f, edge_face, f2idx_to_eidx);
      // std::cout << "\t\t In map idx: "<< eidx << "\n";
      
      // In map
      if (eidx != -1) {
        fidx_to_ccw_eidx[f].push_back(eidx);
      }

      // Not in map, create new edge and map it
      else {
        // Compute edge average vertex
        Eigen::RowVector3d eavg(0.0,0.0,0.0);
        eavg = eavg + Vcur.row(Fcur.row(f)[e1_idx]);
        eavg = eavg + Vcur.row(Fcur.row(f)[e2_idx]);
        eavg = eavg + fidx_to_favg.row(f);
        eavg = eavg + fidx_to_favg.row(edge_face);
        eavg = eavg / 4.0;

        // Push new vertex and map this face to it
        eidx_to_eavg.push_back(eavg);
        fidx_to_ccw_eidx[f].push_back(eidx_to_eavg.size() - 1);

        // Add to f2idx map
        f2idx_to_eidx[std::to_string(f) + ":" + std::to_string(edge_face)] = eidx_to_eavg.size() - 1;
      }

      // std::cout << "\t\tfidx_to_ccw_eidx[f]: ";
      // print_int_vec(fidx_to_ccw_eidx[f]);
    } 
  }
  // print_vec_vec(eidx_to_eavg);
  // std::cout << "eidx_to_eavg size: " << eidx_to_eavg.size() << "\n";

  // std::cout << "fidx_to_ccw_eidx: size (" << fidx_to_ccw_eidx.size() << ")\n";
  // print_double_int_vec(fidx_to_ccw_eidx);








  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////









  // std::cout << "Vertex adjustment: \n";
  // Adjust vertices according to formula
  for (int v = 0; v < Vcur.rows(); v++) {
    // std::cout << "\tVertex idx: " << v << "\n";

    // F, the average of incident face average points
    Eigen::RowVector3d F_avg(0.0, 0.0, 0.0);
    double count = 0.0;

    // Average of adjacent edge midpoints
    Eigen::RowVector3d R_avg(0.0, 0.0, 0.0);

    std::vector<std::set<int>> checked_edges(0); // Tracks edges

    // Loop through incident faces
    // std::cout << "\tLooping thru adj face list: ";
    // print_int_vec(vidx_to_list_fidx[v]);
    for (int vf = 0; vf < vidx_to_list_fidx[v].size(); vf++) {
      int f = vidx_to_list_fidx[v][vf];
      // std::cout << "\t\tIncident Face idx in adjacency list: " << vf << "\n";
      // std::cout << "\t\tActual Face idx: " << f << "\n";
      

      // Add the face's avg to F
      F_avg = F_avg + fidx_to_favg.row(f);
      count++;

      // Loop through incident face vertices
      std::set<int> cur_edge1; // Two edges of f touching v
      std::set<int> cur_edge2;
      for (int i = 0; i < 4; i++) {
        // Waits for the vertex to appear, then computes the two edges
        if (Fcur.row(f)[i] == v) {
          
          if (i == 0) {
            cur_edge1 = {Fcur.row(f)[i], Fcur.row(f)[3]};
            cur_edge2 = {Fcur.row(f)[i], Fcur.row(f)[i+1]};
          } else if (i == 3) {
            cur_edge1 = {Fcur.row(f)[i], Fcur.row(f)[i-1]};
            cur_edge2 = {Fcur.row(f)[i], Fcur.row(f)[0]};
          } else {
            cur_edge1 = {Fcur.row(f)[i], Fcur.row(f)[i-1]};
            cur_edge2 = {Fcur.row(f)[i], Fcur.row(f)[i+1]};
          }
        }
      }

      // std::set<int>::iterator itsldfjgsh = cur_edge1.begin();
      // itsldfjgsh++;
      // std::cout << "\t\tEdge 1: " << *cur_edge1.begin() << ", " << *itsldfjgsh  << "\n";
      // itsldfjgsh = cur_edge2.begin();
      // itsldfjgsh++;
      // std::cout << "\t\tEdge 2: " << *cur_edge2.begin() << ", " << *itsldfjgsh  << "\n";

      // Add edges to list if not already present
      bool cur_edge1_in_list = false;
      bool cur_edge2_in_list = false;
      for (int i = 0; i < checked_edges.size(); i++) {
        if (checked_edges[i] == cur_edge1) {cur_edge1_in_list = true;}
        if (checked_edges[i] == cur_edge2) {cur_edge2_in_list = true;}
      }
      if (!cur_edge1_in_list) {checked_edges.push_back(cur_edge1);}
      if (!cur_edge2_in_list) {checked_edges.push_back(cur_edge2);}

      // std::cout << "\t\tedge1 in, edge2 in: " << cur_edge1_in_list << ", " << cur_edge2_in_list << "\n";
    }

    // Loop through acquired incident edges
    for (int e = 0; e < checked_edges.size(); e++) {
      // Get edge vertex indices into V from the set
      std::set<int>::iterator it = checked_edges[e].begin();
      int v1_idx = *it;
      it++;
      int v2_idx = *it;

      // Compute midpt
      Eigen::RowVector3d midpt(0.0, 0.0, 0.0);
      midpt = midpt + Vcur.row(v1_idx);
      midpt = midpt + Vcur.row(v2_idx);
      midpt = midpt / 2.0;

      // Add midpt to R
      R_avg = R_avg + midpt;
    }
    // Get average of midpts
    R_avg = R_avg / ((double)checked_edges.size());
    // Get average of face averages
    F_avg = F_avg / count;

    // Add new vertex
    Eigen::RowVector3d new_vert = (F_avg + (2 * R_avg) + ((count - 3) * Vcur.row(v))) / count;
    SV.row(v) = new_vert;
  }




  // std::cout << "here\n";




  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










  // Add all face avg vertices to SV
  for (int f = 0; f < Fcur.rows(); f++) {
    SV.conservativeResize(SV.rows() + 1, Eigen::NoChange);
    SV.row(SV.rows() - 1) = fidx_to_favg.row(f);
  }
  // Add all edge avg vertices to SV
  for (int e = 0; e < eidx_to_eavg.size(); e++) {
    SV.conservativeResize(SV.rows() + 1, Eigen::NoChange);
    SV.row(SV.rows() - 1) = eidx_to_eavg[e];
  }









  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////













  // Compute faces
  for (int f = 0; f < Fcur.rows(); f++) {
    std::vector<int> eavg_idxs = fidx_to_ccw_eidx[f];
    Eigen::RowVector4i vert_idxs = Fcur.row(f);

    SF.row(f * 4 + 0) = Eigen::RowVector4i(vert_idxs[0], eavg_idxs[0] + Vcur.rows() + Fcur.rows(), 
                                           f + Vcur.rows(), eavg_idxs[3] + Vcur.rows() + Fcur.rows());

    SF.row(f * 4 + 1) = Eigen::RowVector4i(eavg_idxs[0] + Vcur.rows() + Fcur.rows(), vert_idxs[1],
                                           eavg_idxs[1] + Vcur.rows() + Fcur.rows(), f + Vcur.rows());
                                    
    SF.row(f * 4 + 2) = Eigen::RowVector4i(f + Vcur.rows(), eavg_idxs[1] + Vcur.rows() + Fcur.rows(), 
                                           vert_idxs[2], eavg_idxs[2] + Vcur.rows() + Fcur.rows());

    SF.row(f * 4 + 3) = Eigen::RowVector4i(eavg_idxs[3] + Vcur.rows() + Fcur.rows(), f + Vcur.rows(), 
                                           eavg_idxs[2] + Vcur.rows() + Fcur.rows(), vert_idxs[3]);
  }
  ////////////////////////////////////////////////////////////////////////////
}
