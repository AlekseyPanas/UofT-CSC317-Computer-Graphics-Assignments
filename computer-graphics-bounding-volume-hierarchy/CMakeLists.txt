cmake_minimum_required(VERSION 2.6)
project(boundaryvolumehierarchy)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

### libIGL options: choose between header only and compiled static library
option(LIBIGL_USE_STATIC_LIBRARY     "Use libigl as static library" OFF)
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)

find_package(LIBIGL REQUIRED)
find_package(CGAL REQUIRED)

# Add your project files
include_directories("include/")
if(USE_SOLUTION)
  file(GLOB SRCFILES solution/*.cpp)
else()
  file(GLOB SRCFILES src/*.cpp)
endif()

add_library(core ${SRCFILES})
target_link_libraries(core igl::core igl::opengl igl::opengl_glfw CGAL::CGAL)

add_executable(rays "rays.cpp")
target_link_libraries(rays core igl::core igl::opengl igl::opengl_glfw CGAL::CGAL)

add_executable(distances "distances.cpp")
target_link_libraries(distances core igl::core igl::opengl igl::opengl_glfw CGAL::CGAL)

add_executable(intersections "intersections.cpp")
target_link_libraries(intersections core igl::core igl::opengl igl::opengl_glfw CGAL::CGAL)

add_executable(tri_tri_intersect_test "tri_tri_intersect_test.cpp")
target_link_libraries(tri_tri_intersect_test core igl::core igl::opengl igl::opengl_glfw CGAL::CGAL)
