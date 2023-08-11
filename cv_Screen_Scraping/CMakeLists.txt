project(cv_Screen_Scraping)
cmake_minimum_required(VERSION 3.15)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(cv_Screen_Scraping cv_Screen_Scraping.cpp)
target_link_libraries(cv_Screen_Scraping ${OpenCV_LIBS})
