
cmake_minimum_required(VERSION 2.8)

PROJECT(avanigadda)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(avanigadda MACOSX_BUNDLE avanigadda.cxx )

target_link_libraries(avanigadda ${VTK_LIBRARIES})
