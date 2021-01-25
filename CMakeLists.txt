#
# RASTER-FONT
# 
cmake_minimum_required(VERSION 3.5)

project(Raster-Font C CXX)

add_executable(Raster-Font main/app_main.cpp)

include_directories(main/include main/fonts)

find_library(graph libgraph)
#add_library(graph SHARED IMPORTED) # or STATIC instead of SHARED

#target_link_libraries(graph INTERFACE "/usr/lib/libgraph.so")


target_link_libraries(Raster-Font graph)

