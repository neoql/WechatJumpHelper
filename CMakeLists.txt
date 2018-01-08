cmake_minimum_required(VERSION 3.9)
project(JumpHelper)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})
link_directories(${JSONCPP_LIBRARY_DIRS})

add_executable(JumpHelper JumpHelper.cpp Config.cpp Config.h common.h)
target_link_libraries(JumpHelper ${OpenCV_LIBS})
target_link_libraries(JumpHelper ${JSONCPP_LIBRARIES})

if(UNIX)
    install(TARGETS JumpHelper RUNTIME DESTINATION bin)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/config/ DESTINATION /etc/WechatJumpHelper)
else()
	install(TARGETS JumpHelper RUNTIME DESTINATION .)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/config/ DESTINATION config)
endif()
