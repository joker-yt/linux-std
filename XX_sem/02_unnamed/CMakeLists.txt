project(sem-test)

#cmake_minimum_required(VERSION 3.2.2)

set(sem-test-VERSION_MAJOR 1)
set(sem-test-VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

add_executable(mainbin ${PROJECT_SOURCE_DIR}/src/main.cpp) 
target_link_libraries(mainbin pthread rt)
