cmake_minimum_required(VERSION 3.15)
project(dinosaur)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS REQUIRED)
find_package(Threads REQUIRED)
find_library(LIBRT rt REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_libraries(${CMAKE_THREAD_LIBS_INIT} ${LIBRT} ${OpenCV_LIBS})

add_executable(dinosaur src/main.cpp src/producerprocess.cpp src/filterprocess.cpp src/controlprocess.cpp )


# target_link_libraries(dinosaur PRIVATE ${OpenCV_LIBS} Threads::Threads rt)