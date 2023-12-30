#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <igl/opengl/glfw/Viewer.h>
#include "triangle_triangle_intersection.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Triangle_3 Triangle;

int main(int argc, char *argv[]) {
    int num_tests = argc>1?std::stoi(argv[1]):10000;
    double triangle_spread = argc>2?std::stod(argv[2]):2.0;
    bool show_viewer_for_each = argc>3?(std::stoi(argv[3]) != 0):false;

    std::cout << "\nRunning " << num_tests << " random triangle tests with random point range [-" 
    << triangle_spread << ", " << triangle_spread << 
    "]\n Specify ./tri_tri_intersect <num_tests> <spread> <show_viewer_for_each> for custom values\n" <<
    "Viewer only shown if an inconsistency is found. To show viewer after EACH test, pass 1 for show_viewer_for_each\n\n";

    Eigen::MatrixXi indices;
    indices.resize(1,3);
    indices << 0, 1, 2;

    Eigen::MatrixXd t1_pts;
    Eigen::MatrixXd t2_pts;

    bool is_wrong = false;

    for (int i = 0; i < num_tests; i++) {
        // Set random seed
        //srand((unsigned int) time(0));
        
        // Generate 2 random triangles
        t1_pts =  Eigen::MatrixXd::Random(3, 3) * triangle_spread;
        t2_pts =  Eigen::MatrixXd::Random(3, 3) * triangle_spread;

        bool our_res = triangle_triangle_intersection(t1_pts.row(0), t1_pts.row(1), t1_pts.row(2),
                                                      t2_pts.row(0), t2_pts.row(1), t2_pts.row(2));

        Triangle t1(Point(t1_pts.row(0)[0], t1_pts.row(0)[1], t1_pts.row(0)[2]), 
                   Point(t1_pts.row(1)[0], t1_pts.row(1)[1], t1_pts.row(1)[2]), 
                   Point(t1_pts.row(2)[0], t1_pts.row(2)[1], t1_pts.row(2)[2]));

        Triangle t2(Point(t2_pts.row(0)[0], t2_pts.row(0)[1], t2_pts.row(0)[2]), 
                   Point(t2_pts.row(1)[0], t2_pts.row(1)[1], t2_pts.row(1)[2]), 
                   Point(t2_pts.row(2)[0], t2_pts.row(2)[1], t2_pts.row(2)[2]));

        bool cgal_res = CGAL::do_intersect(t1, t2);

        if (cgal_res != our_res) {
            is_wrong = true;
            std::cout << "Inconsistency Found at i=" << i << ":\n\tCGAL: " << cgal_res << "\n\tOUR: " << our_res << "\n\n";
            break;
        } else if (show_viewer_for_each) {
            std::cout << "Correct Test Result for i=" << i << ":\n\tCGAL: " << cgal_res << "\n\tOUR: " << our_res << "\n\n";
            std::cout.setstate(std::ios::failbit); // Breaks cout so that igl viewer doesnt print instructions each time
            igl::opengl::glfw::Viewer v;
            v.data().set_mesh(t1_pts, indices);
            v.data().set_colors(Eigen::RowVector3d(255,0,0)/255.);
            v.append_mesh();
            v.data().set_mesh(t2_pts, indices);
            v.data().set_colors(Eigen::RowVector3d(0,255,0)/255.);
            v.launch();
            std::cout.clear(); // Fixes cout
        }
    }
    
    if (is_wrong) {
        igl::opengl::glfw::Viewer v;
        v.data().set_mesh(t1_pts, indices);
        v.data().set_colors(Eigen::RowVector3d(255,0,0)/255.);
        v.append_mesh();
        v.data().set_mesh(t2_pts, indices);
        v.data().set_colors(Eigen::RowVector3d(0,255,0)/255.);
        v.launch();
    } else {
        std::cout << "All " << num_tests << " tests passed!!\n\n";
    }
}