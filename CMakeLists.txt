cmake_minimum_required(VERSION 3.15)
project(dinosaur)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(dinosaur src/main.cpp src/producerprocess.cpp src/filterprocess.cpp src/sharedmemory.cpp )


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(dinosaur PRIVATE ${OpenCV_LIBS} Threads::Threads rt)