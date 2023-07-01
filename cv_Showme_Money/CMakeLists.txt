project(cv_Showme_Money)
cmake_minimum_required(VERSION 3.15)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(cv_Showme_Money cv_Showme_Money.cpp)
target_link_libraries(cv_Showme_Money ${OpenCV_LIBS})
