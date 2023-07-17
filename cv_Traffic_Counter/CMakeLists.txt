project(cv_Traffic_Counter)
cmake_minimum_required(VERSION 3.15)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(cv_Traffic_Counter cv_Traffic_Counter.cpp)
target_link_libraries(cv_Traffic_Counter ${OpenCV_LIBS})
