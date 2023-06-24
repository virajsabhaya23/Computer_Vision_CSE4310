project(Raster_Graphic_Editor)
cmake_minimum_required(VERSION 3.15)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(cv_Raster_Graphic_Editor cv_Raster_Graphic_Editor.cpp)
target_link_libraries(cv_Raster_Graphic_Editor ${OpenCV_LIBS})
