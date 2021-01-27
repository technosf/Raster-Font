#
# RASTER-FONT
# 
cmake_minimum_required(VERSION 3.5)

project(Raster-Font C CXX)

set(SOURCES
    main/FontManager.cpp 
    main/fonts.c
)

include_directories(main/fonts main/include)

add_executable(Raster-Font main.cpp ${SOURCES})

find_library(graph libgraph)

target_link_libraries(Raster-Font graph)

