
cmake_minimum_required(VERSION 2.8)
 
PROJECT(NetworkVisualizer)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(NetworkVisualizer MACOSX_BUNDLE NetworkVisualizer.cxx )
 
target_link_libraries(NetworkVisualizer ${VTK_LIBRARIES})
