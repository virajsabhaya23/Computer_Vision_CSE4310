project(a1)
cmake_minimum_required(VERSION 3.15)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(a1 program1.cpp)
target_link_libraries(a1 ${OpenCV_LIBS})
