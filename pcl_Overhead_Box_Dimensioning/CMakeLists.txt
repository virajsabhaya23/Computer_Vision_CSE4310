cmake_minimum_required(VERSION 3.15)
project(pcl_Overhead_Box_Dimensioning)

# set build type to release
set(CMAKE_BUILD_TYPE "Release")

# explicitly set c++11
set(CMAKE_CXX_STANDARD 11)

# configure PCL
find_package(PCL 1.8.0 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable (pcl_Overhead_Box_Dimensioning pcl_Overhead_Box_Dimensioning.cpp CloudVisualizer.cpp)
target_link_libraries (pcl_Overhead_Box_Dimensioning ${PCL_LIBRARIES})