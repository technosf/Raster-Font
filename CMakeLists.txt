#
# RASTER-FONT
# 
cmake_minimum_required(VERSION 3.5)

project(Raster-Font C CXX)

include_directories(main/includes main/fonts)

add_executable(RASTER-FONT main/app_main.cpp)

find_library(graph libgraph.so)

add_library(graph SHARED IMPORTED) # or STATIC instead of SHARED

target_link_libraries(RASTER-FONT graph)

