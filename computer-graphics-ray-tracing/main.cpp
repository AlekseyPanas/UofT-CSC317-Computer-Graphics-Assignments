#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>

/**
 * Get matrix to rotate a vector by the given angle around the "up" (y) axis
 * */ 
Eigen::MatrixXd get_y_rot_matrix(double angle) {
  Eigen::MatrixXd rotation_matrix;
  rotation_matrix.resize(3, 3);
  rotation_matrix << std::cos(angle), 0, std::sin(angle), 0, 1, 0, -std::sin(angle), 0, std::cos(angle);
  return rotation_matrix;
}


/**
 * rotate the camera vectors by a given matrix
*/
void rotate_camera(Camera & cam, Eigen::MatrixXd & rot_matrix) {
  cam.u = rot_matrix * cam.u;
  cam.v = rot_matrix * cam.v;
  cam.w = rot_matrix * cam.w;
}


int main(int argc, char * argv[])
{
  Camera camera;
  std::vector< std::shared_ptr<Object> > objects;
  std::vector< std::shared_ptr<Light> > lights;
  // Read a camera and scene description from given .json file
  read_json(
    // argc<=1?"../src/creative.json":argv[1],
    // argc<=1?"../data/bunny.json":argv[1],
    // argc<=1?"../data/inside-a-sphere.json":argv[1],
    // argc<=1?"../data/sphere-and-plane.json":argv[1],
    argc<=1?"../data/sphere-packing.json":argv[1],
    // argc<=1?"../data/mirror.json":argv[1],
    camera,
    objects,
    lights);

  int width =  640;
  int height = 360;
  std::vector<unsigned char> rgb_image(3*width*height);
  
  // Calculate matrix for rotating camera
  Eigen::MatrixXd rotation_matrix = get_y_rot_matrix(-0.02);

  // Used for spinning the camera around an object
  double spin_theta = 0.0;
  double spin_rad = 12.0;
  Eigen::Vector3d spin_center = Eigen::Vector3d(0, 0.5, 0.5);

  for (int frame=0; frame<60; frame++) {

    // Code to spin around an object
    camera.e = Eigen::Vector3d(spin_rad * std::cos(spin_theta), 0, spin_rad * std::sin(spin_theta));
    Eigen::Vector3d look = (spin_center - camera.e).normalized();
    camera.w = -look;
    camera.u = camera.v.cross(camera.w);
    spin_theta += 0.12;

    std::cout << "Frame #" << frame + 1 << "\n";
    double next_prog = 0.0;

    // For each pixel (i,j)
    for(unsigned i=0; i<height; ++i) 
    {
      for(unsigned j=0; j<width; ++j)
      {
        // Print progress for sanity
        double cur_prog = (((double)i * (double)width + (double)j) / ((double)width * (double)height)) * 100.0;
        if (cur_prog > next_prog) {
          std::cout << cur_prog << "%\n";
          next_prog += 10.0;
        }

        // Set background color
        Eigen::Vector3d rgb(0,0,0);

        // Compute viewing ray
        Ray ray;
        viewing_ray(camera,i,j,width,height,ray);
        
        // Shoot ray and collect color
        raycolor(ray,1.0,objects,lights,0,rgb);

        // Write double precision color into image
        auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
        rgb_image[0+3*(j+width*i)] = 255.0*clamp(rgb(0));
        rgb_image[1+3*(j+width*i)] = 255.0*clamp(rgb(1));
        rgb_image[2+3*(j+width*i)] = 255.0*clamp(rgb(2));

      }
    }
    
    // Rotate view
    // rotate_camera(camera, rotation_matrix);

    // Shift camera
    // camera.e += Eigen::Vector3d(-0.17, 0, 0);

    

    write_ppm(std::to_string(frame) + ".ppm",rgb_image,width,height,3);

  }
}
