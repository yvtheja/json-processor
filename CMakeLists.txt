cmake_minimum_required(VERSION 3.14)
project(JsonObjectProcessor)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

add_executable(JsonObjectProcessor src/main.cpp src/Helper.cpp src/Helper.h src/Entity.h src/JsonHelper.cpp src/JsonHelper.h src/QueryResult.cpp src/QueryResult.h src/BlockingQueue.cpp src/BlockingQueue.h)
target_link_libraries(JsonObjectProcessor ${JSONCPP_LIBRARIES})