cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(ax-box CXX)

include_directories(include)
add_executable(${PROJECT_NAME} src/main.cpp)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install)