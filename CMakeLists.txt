#####################################################################################
#                                                                                   #
#                      CMake Configuration                                          #
#                                                                                   #
#####################################################################################

# Minimum version required
cmake_minimum_required(VERSION 2.8)

# Project Name
project(fll_resize_logos)

# Add c++11 flag
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# OpenCV
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

# Rescale logos
add_executable(scale src/scale.cpp)
target_link_libraries(scale
    ${OpenCV_LIBS}
)
