cmake_minimum_required(VERSION 3.15)
project(dinosaur)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS REQUIRED)
find_package(Threads REQUIRED)
find_package(SFML 2 REQUIRED audio graphics window system)
find_library(LIBRT rt REQUIRED)

if(SFML_FOUND)
    message("eszkereeee")
    include_directories(${SFML_INCLUDE_DIR})
    set(SFML_FLAGS -lsfml-graphics -lsfml-window -lsfml-system)
endif()

include_directories(${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_libraries(${CMAKE_THREAD_LIBS_INIT} ${LIBRT} ${OpenCV_LIBS} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${SFML_FLAGS})
# target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

add_executable(dinosaur src/main.cpp src/producerprocess.cpp src/filterprocess.cpp src/controlprocess.cpp src/gameprocess.cpp src/game/gameprocessV2.cpp src/game/animation.cpp src/game/sheep.cpp)


# target_link_libraries(dinosaur PRIVATE ${OpenCV_LIBS} Threads::Threads rt)