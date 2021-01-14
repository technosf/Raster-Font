#
# RASTER-FONT
# 
cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(Raster-Font C CXX)

add_library(graph SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(graph PROPERTIES
  IMPORTED_LOCATION "/usr/lib/libgraph.so"
  INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include"
)


add_executable(RASTER-FONT main/app_main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack graph)
