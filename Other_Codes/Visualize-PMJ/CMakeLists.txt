cmake_minimum_required(VERSION 2.8)
 
PROJECT(VisualizePMJ)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(VisualizePMJ MACOSX_BUNDLE VisualizePMJ.cxx )
 
target_link_libraries(VisualizePMJ ${VTK_LIBRARIES})