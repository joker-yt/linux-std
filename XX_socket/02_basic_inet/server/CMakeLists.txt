project(inet-socket-server)

cmake_minimum_required(VERSION 3.2.2)

set(inet-socket-VERSION_MAJOR 1)
set(inet-socket-VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_COMPILER "/usr/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

# user defined "INSTALL_PREFIX"
if(NOT DEFINED INSTALL_PREFIX)
        message(FATAL_ERROR "usage: cmake .. -DINSTALL_PREFIX=<install dir> NOT SET \"INSTALL_PREFIX!\"")
endif(NOT DEFINED INSTALL_PREFIX)
set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})

file(GLOB_RECURSE CMAKE_TEST_SRCS ${PROJECT_SOURCE_DIR}/*.cpp)
# refenece pkg-config --libs --cflags
find_package(PkgConfig)
## if you need ....##
#pkg_check_modules(LIBI18N REQUIRED icu-i18n)
include_directories("/home/tk/work/other/plog/include")
#link_directories(${LIBI18N_LIBRARY_DIRS})
#####################

add_executable(${PROJECT_NAME} ${CMAKE_TEST_SRCS})

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin)
