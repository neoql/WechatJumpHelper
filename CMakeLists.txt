cmake_minimum_required(VERSION 3.9)
project(JumpHelper)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

add_executable(JumpHelper JumpHelper.cpp Config.cpp Config.h)
target_link_libraries(JumpHelper ${OpenCV_LIBS})
target_link_libraries(JumpHelper ${JSONCPP_LIBRARIES})