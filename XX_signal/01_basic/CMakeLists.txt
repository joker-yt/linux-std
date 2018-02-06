project(signal-test)

#cmake_minimum_required(VERSION 3.2.2)

set(signal-test-VERSION_MAJOR 1)
set(signal-test-VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

add_executable(mainbin ${PROJECT_SOURCE_DIR}/src/main.cpp
${PROJECT_SOURCE_DIR}/src/ready.cpp)
target_link_libraries(mainbin rt)
