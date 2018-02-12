project(signal-test)

cmake_minimum_required(VERSION 3.2.2)

set(signal-test-VERSION_MAJOR 1)
set(signal-test-VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

link_directories(${PROJECT_SOURCE_DIR}/../../../tool/libs)
include_directories(mytime ${PROJECT_SOURCE_DIR}/../../../tool/include)
add_executable(mainbin ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(mainbin mytime)
