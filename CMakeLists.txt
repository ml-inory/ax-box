cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(ax-box CXX)

set(3RDLIB_ROOT ${CMAKE_SOURCE_DIR}/3rdlibs)

include_directories(${3RDLIB_ROOT}/jsoncpp/include)
include_directories(${3RDLIB_ROOT}/glog/include)
include_directories(include)

add_executable(${PROJECT_NAME} src/main.cpp)
find_library(GLOG_LIBS glog HINTS ${3RDLIB_ROOT}/glog/lib)
find_library(JSONCPP_LIBS jsoncpp HINTS ${3RDLIB_ROOT}/jsoncpp/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC
        ${GLOG_LIBS}
        ${JSONCPP_LIBS}
        pthread
        dl)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install)