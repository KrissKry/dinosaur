cmake_minimum_required(VERSION 3.15)
project(dinosaur)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(dinosaur src/main.cpp)
target_link_libraries(dinosaur ${OpenCV_LIBS})
